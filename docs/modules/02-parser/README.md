# Module 02 — .cub Parser

**Branch:** `main` | **Days:** 3–4

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | This overview |
| [01-cub-format.md](./01-cub-format.md) | The `.cub` file format explained line by line |
| [02-parser-state-machine.md](./02-parser-state-machine.md) | How to parse headers then map in two phases |
| [03-map-validation.md](./03-map-validation.md) | Spawn check, map closure, wall-enclosure algorithm |

**Code references:**
- `code_snippets/parsing/derfleck_checker_1.c` — argument validation + `.cub` extension check
- `code_snippets/parsing/derfleck_checker_2.c` — map character validation + spawn count check
- `code_snippets/parsing/mathias_check_specs.c` — RGB + texture path validation with duplicate detection
- `code_snippets/parsing/mathias_check_map.c` — border-padding + wall check approach

---

## This Module Is a Full Rewrite

Your `so_long` map parser assumed rectangular rows and only checked for specific characters. cub3D is entirely different. **Do not port `so_long/map.c`.**

What you reuse: error handling style, file-reading discipline, general structure.
What you rewrite: everything parser-specific.

---

## Exit Checkpoint

- [x] Valid maps under `maps/m_valid/` parse successfully (`make test-maps`)
- [x] Invalid maps under `maps/m_invalid/` print `Error\n` + message and exit non-zero
- [x] No segfault on tested cases — including maps with varying row lengths / L-shape
- [x] No memory leaks on parser failure / parse-only success (`make valgrind-parser`)
- [x] Parsed `t_game` has textures paths, F/C RGB, padded `map`, `width`, `height` (spawn stays on grid for game)
- [ ] Window + correct facing verified interactively for every valid map (game side)

---

## Crisis Cards (Module 02)

| ⚠️ Symptom | Cause | Fix |
|-----------|-------|-----|
| Segfault on `grid[y][x]` | Row `y` is shorter than `x+1` — rows not padded | Pad all rows to `width` with spaces after parsing |
| Map accepts open walls | Closure check only sees `' '`; spaces inside perimeter miss the check | Recheck your flood-fill or neighbor-check logic |
| `255,255,255 text` accepted as valid RGB | RGB parser doesn't reject trailing content | After parsing 3 values, check remainder is `\0` or `\n` |
| Duplicate `NO` silently accepted | No per-identifier seen-flag | Add `int has_no, has_so...` flags; fail if already 1 |
| Parser hangs at EOF | `get_next_line` returns NULL — your loop doesn't handle it | Test your loop with `while ((line = gnl(fd)) != NULL)` pattern |
| `13_wrong_header_order.cub` unclear | Some subject versions accept any order | Check your specific PDF; err on the side of rejection |

---

## Peer Explanation Drill

1. Walk through the state machine: what triggers the switch from header-phase to map-phase?
2. How does your RGB parser reject `255,255,256`?
3. What is your map closure algorithm? Can it handle spaces inside the map boundary?
4. What happens if the `.cub` file ends immediately after headers with no blank line and no map?
5. Explain: what does `map.width` mean when rows have different lengths?
