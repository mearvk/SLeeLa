# SLeeLa Compression -- build the individual compressor plugins, the registry,
# and the Compression Loader/Unloader/Management Controller into one static
# library, plus the tests. C and C++ sources are compiled with their respective
# toolchains and linked together (the plugins expose C linkage).

CC  ?= cc
CXX ?= c++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS  ?=

BUILD := build

# Core registry + manager + backend registry (C).
CORE_C := src/sleela_compression.c src/sleela_compression_manager.c src/sleela_compression_backend.c

# Every individual algorithm contributes its C plugin (and, where present, its
# C++ detail).
CODEC_C := \
  compressors/store/store.c \
  compressors/rle/rle.c \
  compressors/huffman/huffman.c \
  compressors/lzw/lzw.c \
  compressors/deflate/deflate.c \
  compressors/zlib/zlib.c \
  compressors/gzip/gzip.c \
  compressors/bzip2/bzip2.c \
  compressors/lzma/lzma.c \
  compressors/xz/xz.c \
  compressors/zstd/zstd.c \
  compressors/lz4/lz4.c \
  compressors/brotli/brotli.c \
  compressors/snappy/snappy.c \
  compressors/zip/zip.c

CODEC_CPP := \
  compressors/rle/rle_detail.cpp \
  compressors/huffman/huffman_detail.cpp \
  compressors/lzw/lzw_detail.cpp \
  compressors/deflate/deflate_detail.cpp \
  compressors/zlib/zlib_detail.cpp \
  compressors/gzip/gzip_detail.cpp \
  compressors/bzip2/bzip2_detail.cpp \
  compressors/lzma/lzma_detail.cpp \
  compressors/xz/xz_detail.cpp \
  compressors/zstd/zstd_detail.cpp \
  compressors/lz4/lz4_detail.cpp \
  compressors/brotli/brotli_detail.cpp \
  compressors/snappy/snappy_detail.cpp \
  compressors/zip/zip_detail.cpp

# Per-algorithm include dirs so a plugin's .c finds its own detail header.
CODEC_INC := $(addprefix -I,$(sort $(dir $(CODEC_C))))

ALL_C   := $(CORE_C) $(CODEC_C)
ALL_CPP := $(CODEC_CPP)
OBJS := $(patsubst %,$(BUILD)/%.o,$(ALL_C) $(ALL_CPP))

all: $(BUILD)/libsleela_compression.a

$(BUILD):
	mkdir -p $(BUILD)

# Compile any C source into build/<path>.o
$(BUILD)/%.c.o: %.c | $(BUILD)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CODEC_INC) -c $< -o $@

# Compile any C++ source into build/<path>.o
$(BUILD)/%.cpp.o: %.cpp | $(BUILD)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CODEC_INC) -c $< -o $@

$(BUILD)/libsleela_compression.a: $(OBJS)
	ar rcs $@ $(OBJS)

# Tests (linked with the C++ runtime because the library contains C++ objects).
test: $(BUILD)/test_registry $(BUILD)/test_manager
	./$(BUILD)/test_registry
	./$(BUILD)/test_manager
	@echo "compression: all tests passed"

# The test sources are C, but the library embeds C++ objects, so link via the
# C++ driver. Compile the test TU as C first, then link with g++.
$(BUILD)/test_registry: tests/test_registry.c $(BUILD)/libsleela_compression.a | $(BUILD)
	$(CC) $(CFLAGS) $(CODEC_INC) -c tests/test_registry.c -o $(BUILD)/test_registry.o
	$(CXX) $(BUILD)/test_registry.o $(BUILD)/libsleela_compression.a -o $@

$(BUILD)/test_manager: tests/test_manager.c $(BUILD)/libsleela_compression.a | $(BUILD)
	$(CC) $(CFLAGS) $(CODEC_INC) -c tests/test_manager.c -o $(BUILD)/test_manager.o
	$(CXX) $(BUILD)/test_manager.o $(BUILD)/libsleela_compression.a -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
