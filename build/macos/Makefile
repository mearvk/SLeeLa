# macOS build dispatcher
.PHONY: all core skya slecompiler clean
all: core skya slecompiler
core:
	../../scripts/build-macos.sh
skya:
	../../build/skya-macos.sh
slecompiler:
	../../build/slecompiler-macos.sh
clean:
	$(MAKE) -C ../../impl clean
