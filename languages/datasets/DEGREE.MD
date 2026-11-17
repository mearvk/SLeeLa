# DEGREE: datasets/

Model: staged source data for FRUCHST v1. See [`../AI-AID.md`](../AI-AID.md). Snapshot 2026-11-16.

| Subfolder | Content | Degree |
|---|---|---|
| `frequency-candidates/` | 1,000-word candidate lists plus `DEFINITION-PIPELINE.md` | 51 of 80 profiles staged (64%). 29 have no licensed source. |
| `definition-review/` | Review queues, one per staged profile | Queues exist. 0 senses approved. |
| `definition-pivot/` | EN to FR/ES pivot results | Exists for staged profiles. Homograph errors possible. |
| `definition-english/` | All English Wiktionary senses, ranked | 34,242 of 51,000 words (67.1%) have an English definition. |
| `definition-variants/` | Close-variant English definitions | 5,277 words gained one. Accent-stripped matches carry a caution. |
| `fruchst-input.schema.json` | Generator input schema | Complete for v1. |

Validity: machine-collected suggestions, not approved. Ranked leads are not selections. Overall dataset layer is about 40% of what the model needs; review and binaries are absent.
