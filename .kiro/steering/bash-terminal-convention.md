---
inclusion: always
---

# Bash / terminal source convention

The `bash/` directory is a vendored upstream GNU Bash source tree. Treat it as
reference/vendored code that should stay close to upstream.

**Rule:** When doing new work related to `bash/` (new features, additions,
SLeeLa-specific extensions, or modifications built on top of the Bash tree), put
the new work in the top-level `terminal/` directory rather than editing files in
`bash/` directly.

- Keep `bash/` as the pristine vendored tree wherever practical.
- New/derived work → `terminal/`.
- This keeps SLeeLa's own terminal work clearly separated from upstream Bash and
  easy to review.
