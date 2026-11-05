# SLeeLa language API editions

This directory holds versioned SLeeLa language API documentation.

| Edition | Documentation | Notes |
|---|---|---|
| [3.0](3.0/README.md) | [API edition home](3.0/README.md) | Current normative language edition; new API snapshot generated from the live `/lib` tree. |
| [2.9](2.9/README.md) | [Historical API reference](2.9/index.html) | Previous API edition; preserved. |
| [2.8](2.8/) | Directory | Earlier API edition. |
| [2.6](2.6/) | Directory | Earlier API edition. |
| [2.1](2.1/) | Directory | Earlier API edition. |

The current 3.0 API snapshot is generated with:

```sh
make api-docs
```

The generator reads current `/lib` source comments and declarations. Each
unit's `#sleela` pragma is shown as authored; earlier source pragmas remain
valid and are not silently rewritten.
