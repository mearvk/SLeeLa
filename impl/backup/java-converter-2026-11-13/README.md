# Backup: parser.cpp before the bodiless-method default-value fix (2026-11-13)

`frontend/parser.cpp` is the original from `BASE-COMMIT.txt`, taken before the
change that gives a bodiless (abstract or interface) method returning a
signed or unsigned series integer (`int32`, `uint64`, `int128`, `U256`, ...) a
synthesized `return 0;` instead of `return null;`. The change was needed by the
Java converter (`lib/java/converter`), which spells Java `int` as `int32`.

Restore: `cp impl/backup/java-converter-2026-11-13/frontend/parser.cpp impl/frontend/parser.cpp`,
then regenerate `security/sha256-manifest.json` and rebuild.
Verify the copy: `cd impl/backup/java-converter-2026-11-13 && sha256sum -c SHA256SUMS`.
