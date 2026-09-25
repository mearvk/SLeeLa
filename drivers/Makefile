CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -O2 -pthread
TARGET := libskya-driver-core.a
OBJDIR := build
OBJS := $(OBJDIR)/skya_hardware_profile.o $(OBJDIR)/skya_data_plane.o

all: $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: src/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -Iinclude -c $< -o $@

$(TARGET): $(OBJS)
	ar rcs $@ $^

clean:
	rm -rf $(OBJDIR) $(TARGET)

.PHONY: all clean
