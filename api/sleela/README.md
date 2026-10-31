# SLeeLa language API editions

This directory holds versioned SLeeLa language API documentation.

| Edition | Documentation | Notes |
|---|---|---|
| [2.9](2.9/README.md) | [HTML API home](2.9/index.html) | Current documented snapshot; includes the Philosophy package and its module organization. |
| [2.8](2.8/) | Directory | Earlier API edition. |
| [2.6](2.6/) | Directory | Earlier API edition. |
| [2.1](2.1/) | Directory | Earlier API edition. |

The 2.9 HTML reference is generated from `/lib` with:

```sh
python3 tools/generate-sleeladoc-api.py --out api/sleela/2.9
```

See [the API documentation overview](../README.md) for the overall documentation layout.
