# 04 — Drawing Solid Walls

Once you have exactly how far away the wall is (`perpWallDist`), you need to translate that floating-point distance into integer pixel heights on your 2D monitor.

## The Scaling Math

```mermaid
flowchart TD
    Start[Column X] --> CalcH[Calculate lineHeight<br/> SCREEN_H / perpWallDist]
    CalcH --> StartBound[Calculate drawStart<br/>-lineHeight / 2 + SCREEN_H / 2]
    StartBound --> EndBound[Calculate drawEnd<br/>lineHeight / 2 + SCREEN_H / 2]
    
    EndBound --> Clamp[Clamp drawStart to 0<br/>Clamp drawEnd to SCREEN_H - 1]
    
    Clamp --> Draw[Draw Vertical Line from<br/>drawStart to drawEnd]
    
    Draw --> SaveZ[Save perpWallDist to<br/>ZBuffer[X] for Sprite Rendering]
    SaveZ --> Next[Proceed to Column X + 1]
```

The height of a wall slice on screen is inversely proportional to its distance. 

```c
// Calculate the raw height of line to draw on screen
int lineHeight = (int)(SCREEN_HEIGHT / perpWallDist);
```
If the wall is 1 unit away, it fills the `SCREEN_HEIGHT` perfectly.
If the wall is 4 units away, it is `SCREEN_HEIGHT / 4` pixels tall.

## Clamping to Screen Bounds
A wall that is `0.5` units away will be `SCREEN_HEIGHT * 2`! It is larger than the window. We must calculate the lowest and highest pixel we *actually* intend to draw within our limits.

```c
// Calculate where to start drawing the line (centered vertically)
int drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
if (drawStart < 0) 
    drawStart = 0; // Clamp top

// Calculate where to end the line
int drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
if (drawEnd >= SCREEN_HEIGHT) 
    drawEnd = SCREEN_HEIGHT - 1; // Clamp bottom
```

## The Untextured Drawing Loop
Before worrying about textures, draw a solid color line down the screen.
To fake shading, we typically darken the color depending on whether we hit an X-wall or Y-wall. This gives instant 3D depth perception.

```c
// Base color (e.g., Red)
int color = 0xFF0000;

// Darken if it was a North/South wall
if (side == 1) 
    color = color / 2; // Simple math trick to cut RGB intensity in half

// Draw the vertical line
int y = drawStart;
while (y < drawEnd)
{
    put_pixel(img, x, y, color);
    y++;
}
```

By hooking this up inside your `while(x < SCREEN_W)` loop, you will instantly create an untextured 3D world.
