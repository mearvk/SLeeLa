.PHONY: all install-help install-check
all: install-check
install-help:
	@echo "SLeeLa Quick and Safe Install"
	@echo "  make -C install install-check"
	@echo "  ./quick-safe-install.sh"
	@echo "  ./quick-safe-install-macos.sh"
	@echo "  quick-safe-install.cmd"
install-check:
	@test -f quick-safe-install.sh
	@test -f quick-safe-install-macos.sh
	@test -f quick-safe-install.ps1
	@test -f quick-safe-install.cmd
	@echo "Installer entry points present for Linux, macOS, and Windows 10+."
	@grep -q 'SLEELA_HOME' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer missing SLEELA_HOME/PATH setup"; exit 1; }
	@grep -q 'sleela-env.sh' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer missing PATH snippet"; exit 1; }
	@grep -q 'SLEELA_HOME' quick-safe-install.ps1 || { echo "ERROR: Windows installer missing SLEELA_HOME/PATH setup"; exit 1; }
	@grep -q 'export SLEELA=' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not set SLEELA"; exit 1; }
	@grep -q 'impl/build' quick-safe-install.sh || { echo "ERROR: Linux/macOS installer does not install the built sleela binary"; exit 1; }
	@grep -q 'SLEELA"' quick-safe-install.ps1 || { echo "ERROR: Windows installer does not set SLEELA"; exit 1; }
	@grep -q 'impl.build' quick-safe-install.ps1 || { echo "ERROR: Windows installer does not install the built sleela binary"; exit 1; }
	@echo "PATH + SLEELA_HOME + SLEELA + built-binary install present for all three platforms."
