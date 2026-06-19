# Theory: Sprite Matrix Inversion

To render flat 2D images (like enemies or items) inside a 3D Raycasted world, we cannot use the DDA array-checker algorithm, because Sprites are not bound to strict grid boxes. They exist in floating-point space (e.g. `X: 4.5, Y: 4.5`).

If we want to know *where* on the user's screen integer-wise to draw a sprite, we must project its 2D coordinates through the exact Field-of-View (FOV) Camera Plane.

## Linear Algebra: The Matrix

We have two 2D vectors controlling our raycaster at all times:
- The `Player Direction Vector`
- The `Camera Plane Vector`

Combined, these represent a `2x2` Transformation Matrix.
To map a sprite from the "Global World Space" onto the player's 2D screen, we must multiply the sprite's relative position by the Inverse of this Camera Matrix.

### Step 1: The Relative Position
First, translate the sprite so that the Player is at `(0, 0)`.
```c
double spriteX = duck.pos_x - player.pos_x;
double spriteY = duck.pos_y - player.pos_y;
```

### Step 2: The Inverse Determinant
In Linear Algebra, the constant required to invert a $2x2$ matrix is mathematically defined as $1.0 / (ad - bc)$.
```c
// Determinant = (PlaneX * DirY) - (DirX * PlaneY)
double invDet = 1.0 / (planeX * dirY - dirX * planeY);
```

### Step 3: applying The Inverse Transformation
With our inversion constant, we can now multiply our relative sprite coordinate against our camera vectors to find exactly where the sprite lives *relative to the camera lens itself*.

```c
double transformX = invDet * (dirY * spriteX - dirX * spriteY);
double transformY = invDet * (-planeY * spriteX + planeX * spriteY); 
// Note: transformY is effectively the Z-depth (Distance from camera)
```

## Step 4: Projection to Screen Space
Now that we know the Sprite physically sits `transformY` units away from the lens, and `transformX` units right-or-left of the lens center, we can map it to our 800-pixel wide screen.

```c
// We divide the horizontal offset by the depth to simulate perspective.
// Then we push it to the middle of the 800px screen (WIN_W / 2).
int spriteScreenX = (int)((WIN_W / 2) * (1 + transformX / transformY));
```

The resulting `spriteScreenX` could be `143`. You now know exactly where on the horizontal monitor to begin drawing your Enemy Duck!

## Step 5: Size and Drawing
Since `transformY` is the depth, you just scale `WIN_H / transformY` to find how tall and wide the sprite should be drawn. If depth is `10.0`, the height becomes `60`. Draw a 60x60 pixel box clamped around `spriteScreenX`.

*(Mandatory Z-Buffer Hook: When drawing vertical stripes inside that 60px box, ensure `transformY` is LESS than `ZBuffer[screenX]`, otherwise the duck will render through an obstructing solid wall).*
