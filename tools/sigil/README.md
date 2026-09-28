# Sleela Sigil

Dependency-free (pure Python standard library) generator for the two Sleela
distribution marks:

1. a **configurable QR code** encoding a URL (default: the Sleela GitHub repo), and
2. a **deterministic 248 × 48 steganographic frame** that is idempotent and
   embeds a recoverable digest of the URL.

See [`../../SLEELA.md`](../../SLEELA.md) for the full specification, including the
`.sleela` filetype (**Wrapper™**) reference.

## Quick start

```sh
python3 sigil.py                                 # generate for the resolved URL
python3 sigil.py --url https://your.site/repo    # repoint the QR + frame
python3 sigil.py --verify                        # prove idempotency + recover digest
python3 qr_decode_check.py                       # prove the QR decodes to its URL
```

## Configuring the URL (precedence: high → low)

1. `--url` argument
2. `SLEELA_QR_URL` environment variable
3. `url` in `sigil.config.json` (or `--config <path>`)
4. built-in default `https://github.com/mearvk/Sleela`

## Outputs (`out/`)

| File                       | What                                    |
|----------------------------|-----------------------------------------|
| `sleela-qr.png` / `.txt`   | QR code (PNG + ASCII)                   |
| `sleela-sigil-frame.png` / `.txt` | 248×48 stego frame (PNG + ASCII) |

Requires only Python 3 — no third-party packages, works fully offline.
