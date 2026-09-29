CC ?= cc
CXX ?= c++
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -Werror
CPPFLAGS ?= -Iinclude -Icodecs/include
.PHONY: test clean
test: test-c test-cpp test-sleela
test-c:
	$(CC) $(CFLAGS) $(CPPFLAGS) src/sleela_video.c codecs/src/sleela_video_codecs.c tests/test_video.c -o /tmp/sleela-video-c
	/tmp/sleela-video-c
test-cpp:
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) src/sleela_video.c src/sleela_video.cpp codecs/src/sleela_video_codecs.c codecs/src/sleela_video_codecs.cpp tests/test_video.cpp -o /tmp/sleela-video-cpp
	/tmp/sleela-video-cpp
test-sleela:
	./tests/test_sleela_sources.sh
clean:
	rm -f /tmp/sleela-video-c /tmp/sleela-video-cpp
