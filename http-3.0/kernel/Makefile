# Out-of-tree build for the SLeeLa HTTP 3.0 Station kernel module.
#
# Requires kernel build headers for the running (or target) kernel:
#   /lib/modules/$(uname -r)/build
#
# Usage:
#   make            # build sleela_station.ko
#   sudo make load  # insmod the module (creates /dev/sleela_station)
#   sudo make unload
#   make clean
#
# On a host without kernel headers (e.g. a minimal container) the build will
# report that KDIR is missing; install the matching kernel-headers/-devel
# package first.

KVER ?= $(shell uname -r)
KDIR ?= /lib/modules/$(KVER)/build
PWD  := $(shell pwd)

.PHONY: all clean load unload check-headers

all: check-headers
	$(MAKE) -C $(KDIR) M=$(PWD) modules

check-headers:
	@if [ ! -d "$(KDIR)" ]; then \
	  echo "ERROR: kernel build headers not found at $(KDIR)"; \
	  echo "Install kernel-headers/linux-headers for $(KVER) and retry."; \
	  exit 1; \
	fi

clean:
	-$(MAKE) -C $(KDIR) M=$(PWD) clean 2>/dev/null || rm -f *.o *.ko *.mod *.mod.c .*.cmd modules.order Module.symvers

load: all
	sudo insmod sleela_station.ko

unload:
	sudo rmmod sleela_station
