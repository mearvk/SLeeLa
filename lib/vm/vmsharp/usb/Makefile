# VMSharp(TM) USB boot-media build -- OS-brand dispatcher
# Max Rupplin - MEARVK LLC - 2026
#
# This Makefile STAGES the VMSharp files for USB boot media and, on an EXPLICIT
# and CONFIRMED request, lays them onto a USB device you designate. It detects
# the host OS brand and delegates to Makefile.linux / Makefile.windows /
# Makefile.macos. It is deliberately safe:
#   * `make` (default: stage) touches no device.
#   * `make install` REQUIRES USB_DEVICE=... and I_UNDERSTAND_THIS_ERASES=yes;
#     it never auto-selects a disk and refuses to run otherwise.
#   * It fabricates no firmware binary: VMSharp is modeled in SLeeLa, so the
#     media carries the VMSharp files plus a boot descriptor; a real bootable
#     image is produced only by the pinned OS Creator toolchain.
#
# Engineering preference (the brief's knob): ENGINEERING_PREFERENCE may be
#   sequitur (default) | iq | havingness. It is recorded on the staged media.

ENGINEERING_PREFERENCE ?= sequitur
STAGE_DIR              ?= build/vmsharp-usb
USB_DEVICE             ?=
I_UNDERSTAND_THIS_ERASES ?= no

# ---- OS-brand detection (matches impl/Makefile: OS env / uname -s) ----------
ifeq ($(OS),Windows_NT)
  BRAND := windows
  BRAND_MAKEFILE := Makefile.windows
else
  UNAME_S := $(shell uname -s 2>/dev/null)
  ifeq ($(UNAME_S),Darwin)
    BRAND := macos
    BRAND_MAKEFILE := Makefile.macos
  else
    BRAND := linux
    BRAND_MAKEFILE := Makefile.linux
  endif
endif

export ENGINEERING_PREFERENCE STAGE_DIR USB_DEVICE I_UNDERSTAND_THIS_ERASES

.PHONY: all stage install verify clean help brand

all: stage

# Delegate the real work to the detected brand Makefile.
stage install verify clean:
	@$(MAKE) -f $(BRAND_MAKEFILE) $@

brand:
	@printf 'VMSharp USB media brand: %s (%s)\n' '$(BRAND)' '$(BRAND_MAKEFILE)'
	@printf 'engineering preference: %s\n' '$(ENGINEERING_PREFERENCE)'

help:
	@printf '%s\n' 'VMSharp(TM) USB boot-media build'
	@printf '%s\n' '  make            # stage VMSharp files (no device touched)'
	@printf '%s\n' '  make stage      # same as make'
	@printf '%s\n' '  make verify     # check the staged media layout'
	@printf '%s\n' '  make install USB_DEVICE=<dev> I_UNDERSTAND_THIS_ERASES=yes'
	@printf '%s\n' '                  # write staged media to the DESIGNATED USB device'
	@printf '%s\n' '  make brand      # show detected OS brand + engineering preference'
	@printf '%s\n' '  make clean      # remove the staging dir'
	@printf '%s\n' ''
	@printf '%s\n' 'Detected brand: $(BRAND)  (delegates to $(BRAND_MAKEFILE))'
	@printf '%s\n' 'ENGINEERING_PREFERENCE = sequitur | iq | havingness (current: $(ENGINEERING_PREFERENCE))'
	@printf '%s\n' ''
	@printf '%s\n' 'SAFETY: install never auto-selects a disk; it requires an explicit'
	@printf '%s\n' 'USB_DEVICE and the confirmation flag. VMSharp is a SLeeLa model, so the'
	@printf '%s\n' 'media carries files + a boot descriptor, not a fabricated firmware binary.'
