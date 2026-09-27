# SLeeLa HTTP Packaging

A release package contains the selected HTTP module, headers, configuration examples, build metadata, HTTP.NEGOTIATION.md, VERSION and a SHA-256 manifest.

Native release targets: Linux .deb/.rpm where appropriate; Windows signed MSI/EXE; macOS signed .pkg.

Every release records source commit, SLeeLa HTTP version, target OS and architecture, toolchain, dependencies, SHA-256 manifest and test results.

Repository bootstrap installers are source-aware installers, not substitutes for signed production packages.
