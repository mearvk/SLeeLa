<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa Text Library

Max Rupplin - MEARVK LLC - 2026

The `/lib/text` package is the SLeeLa text library family. The `.sleela` files
(`SLString`, `SLStringBuilder`, `SLStringBuffer`, `SLCharset`, `SLFormatter`,
`SLRegex`, `SLTextReader`, `SLTextWriter`) are the source-level contract; the
native C/C++ layer added here is the implementation boundary for string
**processing, manipulation, and substring** services, below the explicit VM/OS
bridge.

## Native boundary

Following the `/lib/compiler` package convention:

- **C provides the stable ABI** — `include/sleela_string.h`, implemented in
  `src/sleela_string.c`. Pure C11, no dependency beyond the C standard library.
- **C++ orchestrates** — `include/sleela_string.hpp` (the `sleela::text::SleelaString`
  facade) and `src/sleela_string.cpp`. C++ composes the string operations but
  cannot bypass the C ABI's length-explicit, allocation-owning contract.

### ABI contract

- Every input is a length-explicit `(pointer, length)` span, so operations are
  **NUL-tolerant**: embedded NUL bytes are preserved, nothing reads past the
  declared length.
- Producing functions return an **owned** `sleela_string_t` buffer that the
  caller releases with `sleela_string_free()` (or `sleela_string_split_free()`
  for split results). The ABI never silently reuses or frees caller memory.
- Search results use `size_t`; "not found" is `SLEELA_STRING_NPOS`.
- Case folding is ASCII-only, so behaviour is locale-independent and stable.

## Services

| Group | C ABI | C++ facade (`SleelaString`) |
|---|---|---|
| Measure | `length`, `is_empty` | `length`, `empty` |
| Compare | `compare`, `compare_ci`, `equals` | `compare`, `compareIgnoreCase`, `equals` |
| Search | `find`, `rfind`, `count`, `contains`, `starts_with`, `ends_with` | `find`, `rfind`, `count`, `contains`, `startsWith`, `endsWith` |
| Substring | `substring`, `slice_from`, `duplicate` | `substring`, `sliceFrom` |
| Manipulate | `concat`, `trim`/`trim_left`/`trim_right`, `to_upper`/`to_lower`, `reverse`, `replace` | `concat`, `trim`/`trimLeft`/`trimRight`, `toUpper`/`toLower`, `reverse`, `replace` |
| Split/Join | `split`, `split_free` | `split`, `join`, `normalizeWhitespace` |

## Build

From the repository root:

```sh
make text
```

or:

```sh
make -C lib/text
```

This compiles the C and C++ translation units and runs the package sanity
check. The C and C++ objects use distinct `.c.o` / `.cpp.o` stems so both are
always built.

## Example (C)

```c
#include "sleela_string.h"

sleela_string_t out = {0};
const char *s = "the quick brown fox";
sleela_string_substring(s, 19, 4, 5, &out); /* out.data == "quick" */
sleela_string_free(&out);
```

## Example (C++)

```cpp
#include "sleela_string.hpp"
using namespace sleela::text;

SleelaString s("  the quick brown fox  ");
auto words = s.trim().split(" ");       // {"the","quick","brown","fox"}
auto joined = join(words, "-");          // "the-quick-brown-fox"
auto upper = s.trim().toUpper();         // "THE QUICK BROWN FOX"
```

The native layer does not create a parallel string language; it implements the
services the `.sleela` text objects declare.
