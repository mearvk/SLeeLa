# Backup: before Java statements (SLEELA.syntax 43.6 rules 9-10), 2026-11-12

Pre-change copies of the files edited to add the enhanced for (`ForEachStmt`)
and Java numeric literal forms (`L`, `d`/`f`, `_`).

- `frontend/` -- ast.h, parser.{h,cpp}, semantic.cpp, compiler.cpp, keyword_supporter.cpp, driver.cpp, lexer.cpp
- `nordshrift/` -- lowering.cpp, native_method_emit.cpp, sleela_emit.cpp
- `BASE-COMMIT.txt`, `SHA256SUMS` (`sha256sum -c SHA256SUMS`)

Restore: copy back over `impl/`, regenerate `security/sha256-manifest.json`, rebuild.
