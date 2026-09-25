CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2 -pthread
CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2 -pthread
TARGET := libskya-drivers.a
OBJDIR := build
MODEL_CPP := $(shell find . -path './build' -prune -o -path './platform' -prune -o -name driver.cpp -print)
MODEL_C := $(shell find . -path './build' -prune -o -path './platform' -prune -o -name driver.c -print)
MODEL_CPP_OBJS := $(patsubst ./%.cpp,$(OBJDIR)/cpp/%.o,$(MODEL_CPP))
MODEL_C_OBJS := $(patsubst ./%.c,$(OBJDIR)/c/%.o,$(MODEL_C))
OBJS := $(OBJDIR)/skya_driver_registry.o $(OBJDIR)/skya_builtin_drivers.o $(OBJDIR)/skya_vendor_drivers.o $(OBJDIR)/c/skya_driver_runtime.o $(OBJDIR)/c/skya_hardware_io.o $(OBJDIR)/c/skya_hardware_profile.o $(OBJDIR)/c/skya_data_plane.o $(MODEL_CPP_OBJS) $(MODEL_C_OBJS)
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
$(OBJDIR)/cpp/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/c/%.o: %.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_driver_registry.o: src/skya_driver_registry.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_builtin_drivers.o: src/skya_builtin_drivers.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_vendor_drivers.o: src/skya_vendor_drivers.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)
clean:
	rm -rf $(OBJDIR) $(TARGET)
.PHONY: all clean

$(OBJDIR)/c/skya_hardware_profile.o: src/skya_hardware_profile.c | $(OBJDIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/c/skya_data_plane.o: src/skya_data_plane.c | $(OBJDIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@
