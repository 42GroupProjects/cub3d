# Cub3D Parser Audit

Date: 2026-07-19

## Scope

Parser robustness, corner handling, cleanup behavior, and external tester status.

## Findings

1. Empty outer-corner maps were being rejected by the parser even though the game can safely recover if the corner is sealed before raycasting.
2. The parser cleanup path now frees partially built state on failure, so invalid or partially parsed inputs do not leak allocations.
3. The external Cub3D tester invalid suite is passing end-to-end against the current binary.
4. CI is configured to build the project and run the external tester invalid suite.

## Current Behavior

- Empty corners are repaired during map normalization by sealing the outer corner cells with walls.
- Open borders still fail validation.
- A corner-only input now reaches the runtime safely instead of crashing during ray traversal.

## Validation

- `make` passes.
- External tester invalid suite passes 43/43.
- Manual corner-only map check now loads successfully and stays alive in the game loop.

## Notes

- The interactive tester suite is not part of CI because it does not terminate on its own in a headless pipeline.
- Header-style cleanup is still separate from parser behavior and can be handled later if needed.