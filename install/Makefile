.PHONY: all install-help install-check
all: install-check
install-help:
	@echo "SLeeLa Quick and Safe Install"
	@echo "  make -C install install-check"
	@echo "  ./quick-safe-install.sh (interactive user-local/system-wide)"
	@echo "  ./quick-safe-install-macos.sh"
	@echo "  quick-safe-install.cmd"
install-check:
	@test -f quick-safe-install.sh
	@test -f quick-safe-install-macos.sh
	@test -f quick-safe-install.ps1
	@test -f quick-safe-install.cmd
	@grep -q 'Required compiled product missing' quick-safe-install.sh
	@grep -q 'System-wide' README.md
	@echo "Installer entry points and compiled-product installation checks present for Linux, macOS, and Windows 10+."
