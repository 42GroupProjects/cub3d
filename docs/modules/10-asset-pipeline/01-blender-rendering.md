# 01 — Isometric Rendering in Blender

If you want custom enemies for your Combat Lab that look like they belong in a retro 1993 game (like Doom or Duke Nukem), you shouldn't draw them by hand pixel-by-pixel. The industry standard is to create a 3D model in Blender, and render it from 8 different angles to create your 2D sprite sheets.

## 1. Setting up the Orthographic Camera
A standard perspective camera will make a character look distorted if they step to the edge of the frame. You want flat projection.
1. Select your Camera in Blender.
2. In the Camera Properties tab, change "Type" from `Perspective` to `Orthographic`.
3. Adjust the `Orthographic Scale` so your character perfectly fits the frame.

## 2. The 8-Directional Array
You need renders facing North, North-East, East, South-East, South, South-West, West, and North-West. 
1. Create an Empty object at `(0,0,0)` (the center of your character).
2. Parent your Camera to the Empty object.
3. Tilt your camera up by about ~15 degrees to match a standard FPS viewpoint height.
4. Rotate the Empty object around the Z-axis by 45 degrees, render the image, rotate by 45 again, render the image, etc.

## 3. Resolution and Anti-Aliasing (CRITICAL)
Retro engines like Wolfenstein do not handle translucency well. The edge of your sprite should be a hard pixel (either 100% visible or 100% transparent). 

If you leave standard Blender rendering settings on, it will "Anti-alias" the edges, creating semi-transparent pixels that will look like a white/gray halo around your sprite in Cub3D.

1. Go to Render Properties -> Film -> **Transparent** (Check this box).
2. Under Render Properties -> Film -> **Filter Size**: drop this to minimum to reduce anti-aliasing.
3. Render at exactly `64x64` or `128x128` pixels natively. Do not render at 4K and downscale!
