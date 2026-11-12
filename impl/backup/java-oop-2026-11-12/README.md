# Backup: before Java object behavior (SLEELA.syntax 43.6), 2026-11-12

Pre-change copies of the frontend files edited to add subtype assignment,
super(args) / this(args), implicit super(), inherited field initializers,
virtual dispatch, super.m() and bodiless abstract/interface methods.

- `frontend/` -- ast.h, parser.{h,cpp}, semantic.cpp, compiler.cpp, java_references.{h,cpp}
- `BASE-COMMIT.txt` -- commit they were taken from
- verify: `cd impl/backup/java-oop-2026-11-12 && sha256sum -c SHA256SUMS`

Restore: copy `frontend/*` back over `impl/frontend/`, regenerate
`security/sha256-manifest.json`, rebuild.
