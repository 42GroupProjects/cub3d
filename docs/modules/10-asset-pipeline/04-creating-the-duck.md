# 04 — Rendering the 2.5D Duck

If we want the Duck to feel like a real entity in our game—and especially if we want to visually telegraph its attack for the Parry window—we need strict aesthetic controls when pulling the asset from Blender into our C code.

## 1. Blocky vs Smooth Rendering
The original Wolfenstein and DOOM engines did not support high-fidelity 3D shading. They are famous for their pixelated "Voxel/Blocky" sprite style.

When you drop your model into Blender (or MagicVoxel):
1. Scale the Duck model down heavily so features are exaggerated (big beak, blocky wings).
2. Set your material Roughness to 1.0 (Zero reflections). Shiny materials look fake when imported as 2D XPM billboards.
3. Render at `64x64`. 

## 2. The Transparent Centering Trap
When you export an animation, your Duck will waddle. 

If the image bounds are drawn tightly around the Duck on frame 1, but frame 2 shows the Duck leaning forward, exporting a tightly cropped frame 2 will cause the Duck to **shudder horizontally** in your game engine! 

Your C-code calculates the center of the XPM texture based on the exact center of the `<width> x <height>` image file.

### The Fix
In Blender, lock your camera completely. Do not move it. Render all animation states (`Waddle`, `Agro`, `Windup`, `Stunned`) from the exact same camera transform. The `.png` files will have large empty margins of transparency, but the Duck's mathematical center of mass will remain identical across every single file.

## 3. The 4 Mandatory Duck Textures
You need to convert at minimum 4 specific frames using ImageMagick to complete the FSM:
- `duck_waddle.xpm` (Standard stance)
- `duck_windup.xpm` (Beak open aggressively, telegraphing the attack)
- `duck_attack.xpm` (Beak snapping shut)
- `duck_stunned.xpm` (Stars orbiting its head, or visually collapsed)

Import these into your `t_game` struct and swap `entity->current_tex` inside the engine `update_duck_ai` function strictly when the `e_state` changes.
