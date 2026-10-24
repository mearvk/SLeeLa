# Lesson 6 — Status Codes and Testing

Ballum uses explicit result codes. Common cases include:

| Code | Meaning |
|---:|---|
| 0 | OK |
| 1 | Invalid argument |
| 2 | Backend unavailable |
| 3 | Backend error |
| 4 | Display not found |
| 5 | Out of bounds |
| 6 | Buffer too small |
| 7 | Unsupported capability |
| 8 | Permission denied |
| 9 | Not initialized |
| 100 | SLeeLa-side bridge required |

Treat errors as useful information. In particular, `BACKEND_UNAVAILABLE`, `UNSUPPORTED`, and `PERMISSION_DENIED` are not interchangeable: they indicate different next steps.

## Suggested test matrix

- Valid display and supported capability.
- No display / headless CI.
- Unsupported capability.
- Negative coordinates and coordinates exactly equal to width or height.
- Invalid ranges for brightness, warmth, and gamma.
- Native permission denial and backend failure.
- Input idle, event burst, and bounded polling.

Use the native manager unit tests alongside the SLeeLa examples. Do not consider a compile-only test sufficient evidence of device behavior.
