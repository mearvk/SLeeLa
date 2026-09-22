CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
OPENSSL_LIBS ?= -lssl -lcrypto

all: bodi-xml-runner

bodi-xml-runner: bodi_xml_runner.cpp ../email/sleela_email.c ../email/sleela_email.h ../posting/sleela_post.c ../listener/sleela_listener.c ../router/sleela_router.c
	$(CXX) $(CXXFLAGS) -I../email -I../posting -I../listener -I../router bodi_xml_runner.cpp ../email/sleela_email.c ../posting/sleela_post.c ../listener/sleela_listener.c ../router/sleela_router.c $(OPENSSL_LIBS) -o $@

clean:
	rm -f bodi-xml-runner
