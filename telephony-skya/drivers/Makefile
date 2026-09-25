CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2
CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET := libskya-drivers.a
OBJDIR := build
MODEL_CPP := $(shell find . -path './build' -prune -o -path './platform' -prune -o -name driver.cpp -print)
MODEL_C := $(shell find . -path './build' -prune -o -path './platform' -prune -o -name driver.c -print)
MODEL_CPP_OBJS := $(patsubst ./%.cpp,$(OBJDIR)/%.o,$(MODEL_CPP))
MODEL_C_OBJS := $(patsubst ./%.c,$(OBJDIR)/%.o,$(MODEL_C))
OBJS := $(OBJDIR)/skya_driver_registry.o $(OBJDIR)/skya_builtin_drivers.o $(OBJDIR)/skya_vendor_drivers.o $(MODEL_CPP_OBJS) $(MODEL_C_OBJS)
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
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_driver_registry.o: src/skya_driver_registry.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_builtin_drivers.o: src/skya_builtin_drivers.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(OBJDIR)/skya_vendor_drivers.o: src/skya_vendor_drivers.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -Iinclude -c $< -o $@
$(PLATFORM_OBJ): $(PLATFORM_SRC) | $(OBJDIR)
	$(CXX) $(CXXFLAGS) $(PLATFORM_CXXFLAGS) -Iinclude -c $< -o $@
$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)
clean:
	rm -rf $(OBJDIR) $(TARGET)
.PHONY: all clean
