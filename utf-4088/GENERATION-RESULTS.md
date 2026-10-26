# UTF-4088 Million-Entry Generation

This document describes the reproducible generation and the checks required for
a successful catalogue run. It does not pretend a run has succeeded before the
generator has actually completed.

## Generate locally

From the repository root:

```sh
python3 utf-4088/tools/generate_catalogue.py \
  --output utf4088-million.bin \
  --summary utf4088-million.summary.json
```

The default run generates exactly 1,048,576 entries. The binary contains a
28-byte header and 1,048,576 16-byte records, for 16,777,244 bytes total. This
is below the strict 20,000,000-byte file limit.

## Validation gates

A successful run must establish all of the following:

- the exact requested entry count was generated;
- all 8×12 glyph bitmaps are unique within the generated catalogue;
- every input word fits the documented field ranges;
- the binary byte count matches the header and fixed record size;
- the file remains strictly smaller than 20,000,000 bytes;
- the summary includes a SHA-256 digest of the complete file;
- automated tests pass.

The first catalogue uses z-layer 0 and traverses y, x, pressure (1–50), and
normalized voltage (0–255) in deterministic order. The declared input domain has
1,228,800 combinations; the 1,048,576-entry catalogue is its deterministic
prefix. These are model dimensions, not claims of physical voltage sensing or
validated semantic meanings.

## Generated output

The repository workflow generates `utf4088-million.bin` and
`utf4088-million.summary.json` and uploads them as a workflow artifact named
`utf4088-million-catalogue`. This keeps generated binary data out of Git
history while making the complete catalogue downloadable from a successful
workflow run. The summary's SHA-256 is the integrity value for that run.

UTF-4088 remains an experimental SLeeLa format and is not compatible with or
endorsed by the Unicode Standard.
