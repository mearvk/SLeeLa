# Linux build dispatcher
.PHONY: all core skya slecompiler clean
all: core skya slecompiler
core:
	../../scripts/build-linux.sh
skya:
	../../build/skya-linux.sh
slecompiler:
	../../build/slecompiler-linux.sh
clean:
	$(MAKE) -C ../../impl clean
