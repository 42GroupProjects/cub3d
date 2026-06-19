# 00-03 — Subject Rules

The 42 cub3D subject has very specific requirements. Read every section here before starting.

---

## Mandatory Requirements

| Requirement | Exact specification |
|-------------|---------------------|
| Library | Must use **MiniLibX** only |
| Window | Must not freeze, minimize correctly, support ESC and red-X close |
| Textures | N/S/E/W walls must each use a **different** texture |
| Floor/Ceiling | Must be solid **RGB colors** (no texture required for mandatory) |
| Movement | **W/A/S/D** — W=forward, S=backward, A=strafe-left, D=strafe-right |
| Rotation | **Left/Right arrow keys** (mouse look is bonus) |
| Scene file | Must parse a `.cub` file passed as the sole argument |
| Spaces in map | Spaces inside the map are **valid void content** — not errors |
| Error handling | Any invalid input prints `Error\n` + message and exits non-zero |
| README | Written in English with an **italic first line** |

## What Needs To Be Rejected

Your parser must reject:

- Missing or duplicate header identifiers (`NO`, `SO`, `WE`, `EA`, `F`, `C`)
- RGB values outside 0–255 or containing non-numeric characters
- Maps with no spawn point or more than one spawn point
- Maps that are not fully enclosed by walls (any floor cell that leaks to the map border)
- Unknown characters in the map body
- Files with wrong extension (anything not `.cub`)
- Missing arguments or too many arguments

## What Becomes Mandatory When You Collide with Walls

> **Important:** The subject lists "wall collisions" under **bonus**. Your mandatory submission only needs to prevent clipping (a simple wall-stop check). Full sliding collision is bonus.

## What Is Bonus (Only After Mandatory Is Perfect)

Official bonus list (subject verbatim):

1. Wall collisions (sliding, not wall-stop)
2. A minimap
3. Doors
4. Animated sprites
5. Mouse rotation

> **"The bonus part will only be assessed if the mandatory part is PERFECT."**

## Branch Policy

```
main       → mandatory only. never merge from bonus or combat-lab.
bonus      → official bonus features only. branch from main after freeze.
combat-lab → experiments (duck, sword, parry, HUD). branch from bonus after Day 15.
```

## README Format

The subject requires your final `README.md` to include:

```markdown
*A raycasting 3D engine built in C using MiniLibX...*   ← italic first line, required

## Description
...

## Build & Run
make
./cub3d maps/test.cub

## Controls
| Key | Action |
...

## Map Format
...

## Resources
...

## AI Usage
[Explain which sections used AI, how you verified them, peer review done.]
```

## AI Usage Policy

From the 42 subject AI section:

1. Use AI only for content you **fully understand**
2. Seek **peer review systematically**
3. **Declare** AI assistance explicitly
4. Evaluators **may ask you to explain or modify** any AI-assisted code

**In practice:**

- Log every AI session in `docs/journal/ai_usage_log.md`
- Both partners review every AI output before it enters any source file
- Never commit code you cannot explain line-by-line under oral questioning
