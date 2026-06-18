# vscode-debugging.md — Step-Through Debugging for Cub3D

Using `printf` to debug a DDA loop that runs 800 times per frame is not a debugging strategy—it is a suicide mission. This guide sets up VS Code LLDB (macOS) and GDB (Ubuntu) so you can step through the `while (hit == 0)` DDA loop frame-by-frame like a professional engineer.

## macOS Setup (LLDB via CodeLLDB extension)

### 1. Install Prerequisites
Open VS Code and install the extension **CodeLLDB** (by Vadim Chugunov).

### 2. Compile with Debug Symbols
Make sure your Makefile preserves debug symbols.
```makefile
CFLAGS = -Wall -Wextra -Werror -g   # -g is critical
```

### 3. Create `.vscode/launch.json`
In your project root, create this file:
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "type": "lldb",
      "request": "launch",
      "name": "Debug cub3D",
      "program": "${workspaceFolder}/cub3D",
      "args": ["${workspaceFolder}/maps/debug.cub"],
      "cwd": "${workspaceFolder}",
      "stopOnEntry": false,
      "env": {}
    }
  ]
}
```

### 4. Workflow Example: Debugging the DDA Corner Crash
1. Open `src/engine/raycaster.c`.
2. Click on the line gutter next to `while (hit == 0)` to set a **red breakpoint dot**.
3. Press `F5` to launch the debugger. The game opens an MLX window.
4. Walk your player toward the corner where it normally crashes.
5. VS Code will **pause the entire process** exactly when the DDA loop starts for that problematic column.
6. Use the **Variables** panel to inspect `map_x`, `map_y`, `side_dist_x`, `side_dist_y` frame by frame.
7. Press `F10` (Step Over) to advance one line at a time.

## Ubuntu Setup (GDB)

### 1. Create `.vscode/launch.json`
```json
{
  "version": "0.2.0",
  "configurations": [
    {
      "type": "cppdbg",
      "request": "launch",
      "name": "Debug cub3D (GDB)",
      "program": "${workspaceFolder}/cub3D",
      "args": ["${workspaceFolder}/maps/debug.cub"],
      "cwd": "${workspaceFolder}",
      "MIMode": "gdb",
      "miDebuggerPath": "/usr/bin/gdb",
      "setupCommands": [
        {
          "description": "Enable pretty-printing",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ]
    }
  ]
}
```
Requires the **C/C++** extension (by Microsoft) on Ubuntu.

## Key Breakpoints to Set for Cub3D

| Location | What to Inspect |
|---|---|
| Start of `while (hit == 0)` | `map_x`, `map_y` — Guard against going out of bounds |
| After `side_dist_x < side_dist_y` check | `side_dist_x`, `delta_dist_x` — See exact stepping math |
| `render_wall_stripe()` entry | `perp_wall_dist`, `line_height` — Confirm fisheye math |
| `get_tex_color()` entry | `tex_x`, `tex_y` — Spot mirroring/flipping bugs |
| Inside `clean_exit()` | All `t_img *` pointers — Confirm none are NULL before `mlx_destroy_image` |
