# Capstone 09: The Duck Duel

## The Objective
You must tie the Weapon Animation overlay together with the 3D projected Enemy State Machine to create a functional combat encounter. The user must be able to parry an incoming Duck attack, and subsequently kill the stunned Duck.

## Requirements Checklist
1. [ ] Create a `t_entity` struct representing the Duck. Place it in the map inside a long hallway.
2. [ ] Implement the Distance Scaling and Z-buffer inverse projection (from Module 08) so the Duck draws to the screen correctly.
3. [ ] Implement the `update_duck_ai` logic explicitly using the `DUCK_WINDUP` (0.6 seconds) state window.
4. [ ] Overlay your Weapon Sprite. Give the Weapon an `IDLE` state and a `PARRY` state (triggered by Right Click).
5. [ ] Pass the `game` time `dt` into your loop so timers decrement accurately.
6. [ ] Implement the collision code: If Player is Parrying AND Duck is winding up AND Angle/Distance is correct -> Transition Duck to `DUCK_STUNNED`.
7. [ ] **Execution:** Launch the game. Walk toward the duck. It should charge you. Wait for it to pull back its beak, hit Right Click, and observe it freeze in a stunned animation. Left click to destroy it.

## Verification
- **Does the Duck seem to skip the windup?** Ensure your `delta_time (dt)` is correctly calculating the exact fraction of a second between frames (e.g. `0.016` for 60fps). If you decrement `timer--` every frame without `dt`, 0.6 will hit zero in less than one visual frame!
- **Did you parry but still take damage?** You forgot to cancel the damage function. Make sure your logic stops checking for `ATTACK` hits if the state is successfully switched to `STUNNED`.

This Capstone completes the physical programming required for the entire Advanced Mechanics phase.
