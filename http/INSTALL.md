# SLeeLa HTTP Installation

SLeeLa HTTP 1.0 through 9.0 use one common installation model. HTTP 1.0/1.1 remain compatibility targets; SLeeLa 2.0 through 9.0 are project-specific generations.

Linux/macOS: run http/install/sleela-http-install.sh --version 9.0 --source-root .

Windows PowerShell: run http/install/sleela-http-install.ps1 -Version 9.0 -SourceRoot .

Use --all / -All to install all available module trees. The installer detects OS and architecture, validates the module, runs available syntax checks, installs source and metadata, and records a manifest. It does not automatically open firewall ports or start services.

Default locations: Linux/macOS /usr/local/share/sleela/http; Windows %ProgramFiles%\\SLeeLa\\http.

For production, use signed native packages described in PACKAGING.md.
