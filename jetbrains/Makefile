# SLeeLa JetBrains source acquisition helpers
# Max Rupplin - MEARVK LLC - 2026

.PHONY: all download download-macos download-windows help

all: help

download:
	bash ./download-source.sh

download-macos:
	bash ./download-source-macos.sh

download-windows:
	@echo "Use PowerShell on Windows:"
	@echo "  powershell -ExecutionPolicy Bypass -File .\\download-source.ps1"

help:
	@printf '%s\n' 'SLeeLa JetBrains source acquisition'
	@printf '%s\n' '  make download         Acquire IntelliJ Community source on Unix/Linux'
	@printf '%s\n' '  make download-macos   Acquire IntelliJ Community source on macOS'
	@printf '%s\n' '  make download-windows Show the Windows PowerShell command'
