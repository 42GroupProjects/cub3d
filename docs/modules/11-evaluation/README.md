# Module 11 — Evaluation and Defense

**Branch:** All | **Ongoing throughout the course**

---

## Files in This Module

| File | Contents |
|------|---------|
| [README.md](./README.md) | Defense drills, templates |

---

## Evaluation Drill — Run for Both Partners

Both students without notes:

| Task | Time limit |
|------|-----------|
| Explain parser flow from `argv[1]` to populated `t_map` | 2 min |
| Explain DDA: deltaDistX, sideDistX, perpWallDist in order | 2 min |
| Explain how texture selection works (which branch, which side) | 1 min |
| Explain cleanup chain: what frees in what order | 1 min |
| Add live debug feature (e.g. print `pos_x, pos_y` each frame) | 10 min |
| Introduce then fix pixel-trail bug (comment out `ft_memset`) | 5 min |
| Justify one AI-assisted section in the codebase | 2 min |

---

## Question Bank (from `docs/reference/`)

Primary brief (our methodology + answers + drills):

- [`docs/reference/methodology-oral-defense.md`](../../reference/methodology-oral-defense.md)

Also practice aloud from:

- [`docs/reference/evaluation-questions.md`](../../reference/evaluation-questions.md)
- [`02-defense-qna.md`](./02-defense-qna.md) (math-focused)

---

## Journal Templates

Use these every session:

```
docs/journal/daily-template.md
docs/journal/pair-review-template.md
docs/journal/weekly-retrospective.md
docs/journal/ai_usage_log.md
```

---

## AI Usage Log Format

Every AI session:

```md
## Entry: [Date]
Chapter / File: [which module or file]
What was AI asked to help with:
What AI produced:
What was verified or changed:
What would you do if the AI output is wrong:
Peer review: [yes / no — reviewed by whom]
```

---

## Exit Criteria (Both Partners)

- [ ] Both can explain parser flow without notes
- [ ] Both can explain DDA without notes
- [ ] Both can explain texture selection without notes
- [ ] Both can explain cleanup chain without notes
- [ ] Both have made a live code modification in drill practice
- [ ] `tests/evaluator_checklist.md` is 100% complete
- [ ] `docs/journal/ai_usage_log.md` has an entry for every AI interaction
- [ ] README final and both agree on the AI usage section content
