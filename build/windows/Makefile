# Windows 10+ build dispatcher
# Intended for MSYS2/Git Bash environments with PowerShell available.
.PHONY: all core skya slecompiler
all: core skya slecompiler
core:
	powershell.exe -ExecutionPolicy Bypass -File ../../build-windows.ps1
skya:
	powershell.exe -ExecutionPolicy Bypass -File ../skya-windows.ps1
slecompiler:
	powershell.exe -ExecutionPolicy Bypass -File ../slecompiler-windows.ps1
