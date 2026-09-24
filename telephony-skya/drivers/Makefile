CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
TARGET := libskya-drivers.a
OBJDIR := build
OBJS := $(OBJDIR)/skya_driver_registry.o $(OBJDIR)/skya_builtin_drivers.o

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/skya_driver_registry.o: src/skya_driver_registry.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/skya_builtin_drivers.o: src/skya_builtin_drivers.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	ar rcs $@ $(OBJS)

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
