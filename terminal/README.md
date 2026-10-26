<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# terminal/

SLeeLa's own terminal development work.

This is a **regular directory in the SLeeLa repository** — not a submodule. It
holds SLeeLa-specific terminal features, extensions, and work derived from the
vendored GNU Bash tree.

## Convention

Per `.kiro/steering/bash-terminal-convention.md`:

- `bash/` — the pristine, vendored upstream GNU Bash source tree. Keep it close
  to upstream; avoid editing it directly.
- `terminal/` — **new / derived / SLeeLa-specific terminal work goes here.**

Keeping SLeeLa's terminal work in this directory (rather than editing `bash/`)
separates our changes from upstream Bash and keeps them easy to review.

## History

`terminal/` was previously configured as a git submodule pointing at the
upstream GNU Bash repository. That made it appear as a submodule reference on
GitHub and pointed at upstream rather than being SLeeLa's own development
folder. It has been converted into a normal tracked directory so development can
happen directly in-repo.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
