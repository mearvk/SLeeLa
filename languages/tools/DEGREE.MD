# DEGREE: tools/

Model: Python scripts that stage data, build definitions, and build/validate `fruchst.<code>.bin`. See [`../AI-AID.md`](../AI-AID.md). Snapshot 2026-11-16.

| Stage | Scripts | Tests |
|---|---|---|
| Staging | `stage_wordhoard.py`, `stage_frequencywords.py`, `stage_unilex.py` | `test_stage_wordhoard.py` only. The other two stagers have no test file. |
| Definitions | `fruchst_english_round.py`, `fruchst_english_pivot.py`, `fruchst_close_variants.py`, `fruchst_english_segments.py`, `fruchst_lexemes.py`, `fruchst_definition_queue.py`, `fruchst_definition_batch.py` | Tests for english_round, definition_queue, definition_batch. None for pivot, close_variants, segments, lexemes. |
| Build/validate | `fruchst.py` | `test_fruchst.py`, `test_fruchst_production.py` |

Degree: the pipeline up to review queues is implemented. The build step is implemented but has never produced a production binary because no approved input exists. I did not run the tests when writing this file.
