CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET := libskya-drivers.a
OBJDIR := build
OBJS := $(OBJDIR)/skya_driver_registry.o \
        $(OBJDIR)/skya_builtin_drivers.o \
        $(OBJDIR)/skya_vendor_drivers.o

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PLATFORM_OBJ := $(OBJDIR)/skya_linux_driver.o
PLATFORM_SRC := platform/linux/skya_linux_driver.cpp
PLATFORM_CXXFLAGS :=
else ifeq ($(UNAME_S),Darwin)
PLATFORM_OBJ := $(OBJDIR)/skya_macos_driver.o
PLATFORM_SRC := platform/macos/skya_macos_driver.cpp
PLATFORM_CXXFLAGS := -framework CoreAudio
else
PLATFORM_OBJ := $(OBJDIR)/skya_windows_driver.o
PLATFORM_SRC := platform/windows/skya_windows_driver.cpp
PLATFORM_CXXFLAGS :=
endif

OBJS += $(PLATFORM_OBJ)

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/skya_driver_registry.o: src/skya_driver_registry.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/skya_builtin_drivers.o: src/skya_builtin_drivers.cpp include/skya_vendor_drivers.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/skya_vendor_drivers.o: src/skya_vendor_drivers.cpp include/skya_vendor_drivers.h include/skya_phone_driver.h | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PLATFORM_OBJ): $(PLATFORM_SRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(PLATFORM_CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
