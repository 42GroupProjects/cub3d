# Theory: Global Engine Architecture

A stable C graphics engine relies heavily on a strictly defined lifecycle. If you initialize X11/AppKit before validating the map, a parsing error will leave the graphics context stranded in memory. Or worse, if you allocate mlx images without hooking the closure event, the user's only way to escape your game is a hard `Ctrl+C` interrupt.

To ensure flawless memory management, your engine should follow the **"Parse-First, Render-Last"** pattern strictly.

## The Cub3D Lifecycle

```mermaid
flowchart TD
    Start([main]) --> ParseArgs{argc == 2?}
    ParseArgs -- No --> ErrorArg[Write Error: Bad Args] --> Exit[exit(1)]
    ParseArgs -- Yes --> ExtCheck{Ends in '.cub'?}
    ExtCheck -- No --> ErrorExt[Write Error: Bad Extension] --> Exit
    
    ExtCheck -- Yes --> ParseMap[Init Map Struct\n& Run GNL Parser]
    
    ParseMap --> ValMap{Is Map Valid?}
    ValMap -- No --> ErrorMap[Write Error: Invalid Map]
    ErrorMap --> FreeMap[Free Map Linked List] --> Exit
    
    ValMap -- Yes --> MLX[mlx_init]
    MLX --> CreateWin[mlx_new_window]
    CreateWin --> CreateImg[mlx_new_image]
    CreateImg --> CacheTex[mlx_xpm_file_to_image]
    
    CacheTex --> Hook[Register Hooks\nKeypress, Keyrelease, Close]
    
    Hook --> LoopHook[mlx_loop_hook]
    LoopHook --> Loop[mlx_loop]
    
    subgraph "Render Pipeline (60 FPS)"
        LoopHookFrame[game_loop] --> Input[Update Player Position]
        Input --> Raycast[Cast 800 Rays\nDDA Algorithm]
        Raycast --> TexMapping[Sample Textures\nWrite to RAM Buffer]
        TexMapping --> Flush[mlx_put_image_to_window]
    end
    
    Loop -.-> LoopHookFrame
    
    UserAction{User presses ESC\nor clicks Red Cross}
    UserAction --> CleanUp
    
    subgraph "Teardown (clean_exit)"
        CleanUp[Destroy Images] --> CleanWindow[Destroy Window]
        CleanWindow --> CleanDisplay[Linux Only:\nmlx_destroy_display]
        CleanDisplay --> FreeStructs[Free Map Memory]
        FreeStructs --> ExitSuccess([exit(0)])
    end
```

## Architectural Rules

1. **Parser Autonomy**: The map parsing logic must have zero dependencies on `mlx`. If the map is invalid, the program shuts down before a single graphical struct is ever created.
2. **Buffer Modifying**: Inside the `game_loop`, you MUST write directly to the 1D `img.addr` memory string. Calling `mlx_pixel_put()` inside the loop will invoke internal system calls 480,000 times per frame, bringing the engine to a 1FPS crawl.
3. **Cross-Platform Teardown**: `mlx_destroy_display` is only available (and mandatory) on Linux X11 systems. When architecting your `clean_exit`, use `#ifdef __linux__` to prevent macOS AppKit compilation failures.
