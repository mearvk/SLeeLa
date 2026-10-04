# SLeeLa Codecs -- build the individual codec plugins, the registry, and the
# Codec Loader/Unloader/Management Controller into one static library, plus the
# tests. C and C++ sources are compiled with their respective toolchains and
# linked together (the plugins expose C linkage).

CC  ?= cc
CXX ?= c++
CFLAGS   ?= -std=c11   -O2 -Wall -Wextra -Wpedantic -Iinclude
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS  ?=

BUILD := build

# Core registry + manager + backend registry (C).
CORE_C := src/sleela_codecs.c src/sleela_codec_manager.c src/sleela_codec_backend.c

# Every individual codec contributes its C plugin and its C/C++ detail.
CODEC_C := \
  codecs/pcm_wav/pcm_wav.c \
  codecs/aiff/aiff.c \
  codecs/flac/flac.c \
  codecs/alac/alac.c \
  codecs/mp3/mp3.c \
  codecs/aac/aac.c \
  codecs/he_aac/he_aac.c \
  codecs/vorbis/vorbis.c \
  codecs/opus/opus.c \
  codecs/speex/speex.c \
  codecs/wma/wma.c \
  codecs/ac3/ac3.c \
  codecs/eac3/eac3.c \
  codecs/amr_nb/amr_nb.c \
  codecs/amr_wb/amr_wb.c \
  codecs/g711_mulaw/g711_mulaw.c \
  codecs/g711_alaw/g711_alaw.c \
  codecs/midi/midi.c \
  codecs/matroska_audio/matroska_audio.c \
  codecs/webm_audio/webm_audio.c

CODEC_CPP := \
  codecs/pcm_wav/pcm_wav.cpp \
  codecs/g711_mulaw/g711_detail.cpp \
  codecs/g711_alaw/g711_alaw_detail.cpp \
  codecs/aiff/aiff_detail.cpp \
  codecs/flac/flac_detail.cpp \
  codecs/alac/alac_detail.cpp \
  codecs/mp3/mp3_detail.cpp \
  codecs/aac/aac_detail.cpp \
  codecs/he_aac/he_aac_detail.cpp \
  codecs/vorbis/vorbis_detail.cpp \
  codecs/opus/opus_detail.cpp \
  codecs/speex/speex_detail.cpp \
  codecs/wma/wma_detail.cpp \
  codecs/ac3/ac3_detail.cpp \
  codecs/eac3/eac3_detail.cpp \
  codecs/amr_nb/amr_nb_detail.cpp \
  codecs/amr_wb/amr_wb_detail.cpp \
  codecs/midi/midi_detail.cpp \
  codecs/matroska_audio/matroska_audio_detail.cpp \
  codecs/webm_audio/webm_audio_detail.cpp

# Per-codec include dirs so a plugin's .c finds its own detail header.
CODEC_INC := $(addprefix -I,$(sort $(dir $(CODEC_C))))

ALL_C   := $(CORE_C) $(CODEC_C)
ALL_CPP := $(CODEC_CPP)
OBJS := $(patsubst %,$(BUILD)/%.o,$(ALL_C) $(ALL_CPP))

all: $(BUILD)/libsleela_codecs.a

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

$(BUILD)/libsleela_codecs.a: $(OBJS)
	ar rcs $@ $(OBJS)

# Tests (linked with the C++ runtime because the library contains C++ objects).
test: $(BUILD)/test_registry $(BUILD)/test_manager
	./$(BUILD)/test_registry
	./$(BUILD)/test_manager
	@echo "codecs: all tests passed"

# The test sources are C, but the library embeds C++ objects, so link via the
# C++ driver. Compile the test TU as C first, then link with g++.
$(BUILD)/test_registry: tests/test_registry.c $(BUILD)/libsleela_codecs.a | $(BUILD)
	$(CC) $(CFLAGS) $(CODEC_INC) -c tests/test_registry.c -o $(BUILD)/test_registry.o
	$(CXX) $(BUILD)/test_registry.o $(BUILD)/libsleela_codecs.a -o $@

$(BUILD)/test_manager: tests/test_manager.c $(BUILD)/libsleela_codecs.a | $(BUILD)
	$(CC) $(CFLAGS) $(CODEC_INC) -c tests/test_manager.c -o $(BUILD)/test_manager.o
	$(CXX) $(BUILD)/test_manager.o $(BUILD)/libsleela_codecs.a -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
