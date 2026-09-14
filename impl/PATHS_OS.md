# SLeeLa Paths and Filesystem OS Layer

The SLeeLa paths/filesystem layer makes common path and filesystem operations predictable across Linux/POSIX and Windows/Win32.

## API

`core/sleela_path.h` / `core/sleela_path.c` provide:

- platform detection: `slpath_platform`, `slpath_platform_name`
- native separator selection: `slpath_separator`
- path composition: `slpath_join`
- separator normalization: `slpath_normalize`
- absolute-path resolution: `slpath_absolute`
- current working directory: `slpath_current_directory`
- existence checks: `slpath_exists`
- file/directory predicates: `slpath_is_file`, `slpath_is_directory`
- directory creation: `slpath_make_directory`
- removal and rename: `slpath_remove`, `slpath_rename`

The public API uses ordinary UTF-8 C strings and hides platform-specific headers and system calls from callers.

## Platform behavior

Linux uses POSIX filesystem calls and `/` as the native separator. Windows uses Win32/CRT calls and `\\` as the native separator. Both `/` and `\\` are accepted as input separators and normalized to the native separator.

The abstraction is deliberately small. It is intended to become the common boundary for VM/runtime path handling rather than allowing platform-specific filesystem calls to spread through the SLeeLa core.

## Verification

The standalone smoke test is `core/path_platform_smoke.c` and is built by the implementation Makefile as `test-path-platform`.

The existing SHA-256 verification gate remains a prerequisite for object creation and therefore for this test as well.

## Migration rule

New cross-platform SLeeLa code should use `sleela_path.h` instead of directly calling platform-specific path/filesystem APIs. Existing VM filesystem calls can be migrated incrementally without changing the public SLeeLa path API.
