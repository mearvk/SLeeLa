# SLeeLa JetBrains source acquisition helpers
# Max Rupplin - MEARVK LLC - 2026

.PHONY: all check download download-macos download-windows help

# This package currently provides source-acquisition helpers, not a built IDE.
# Keep its checks in the root build without pretending IntelliJ is compiled here.
all: check

check:
	@test -f download-source.sh
	@test -f download-source-macos.sh
	@test -f download-source.ps1
	@test -f README.md
	@echo "JetBrains acquisition helpers: PASS (IDE artifact is not built by this target)."

download:
	bash ./download-source.sh

download-macos:
	bash ./download-source-macos.sh

download-windows:
	@echo "Use PowerShell on Windows:"
	@echo "  powershell -NoLogo -NoProfile -File .\\download-source.ps1"

help:
	@printf '%s\n' 'SLeeLa JetBrains source acquisition'
	@printf '%s\n' '  make download         Acquire IntelliJ Community source on Unix/Linux'
	@printf '%s\n' '  make download-macos   Acquire IntelliJ Community source on macOS'
	@printf '%s\n' '  make download-windows Show the Windows PowerShell command'
