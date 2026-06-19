# Capstone 10: The Custom Asset Pipeline (Duck Edition)

## The Objective
You must prove that you can create your own graphical assets specifically tailored to the Parry-heavy combat loop, and integrate them smoothly into your engine safely. We will discard placeholder assets and formally import the Duck.

## Requirements Checklist
1. [ ] Export your custom Duck character from Blender (or Magic Voxel) into at least 4 states: `waddle`, `windup`, `attack`, `stunned`.
2. [ ] Ensure the camera was locked during export so the Duck does not shift horizontally off-center when alternating states.
3. [ ] Convert the `png` renders to `xpm` preserving transparency metadata.
4. [ ] Export 3 frames of your first-person Weapon (e.g. `idle`, `swing`, `parry`).
5. [ ] Ensure your parser correctly loads all 7 of these `xpm` files into `t_img` wrappers. Ensure your `clean_exit()` frees them all.
6. [ ] Source (or record yourself) two key sound effects: `quack.wav` and `clang.wav`.
7. [ ] Plug the weapon overlays into your screen rendering hook, and the Duck into the `update_duck_ai` FSM logic. 

## Verification
- **Test 1:** Boot the game on macOS and Linux (if accessible via VM or cluster). It should compile effortlessly without linking errors thanks to our `#ifdef` macros in the Makefile.
- **Test 2:** Get in range of the duck. As it opens its beak, you should hear `quack.wav` fire perfectly synchronized with the FSM transition. Right-click to Parry. You should hear `clang.wav` fire perfectly synchronized with your Weapon overlay turning sideways.
- **Test 3:** Press the X button to close the game. Run your rigorous memory leak checker. 0 Bytes leaked.

Passing this Capstone means you have a fully functional game looping inside a memory-safe, cross-platform engine.
