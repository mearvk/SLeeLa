CXX ?= c++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
OPENSSL_LIBS ?= -lssl -lcrypto

all: bodi-xml-runner

bodi-xml-runner: bodi_xml_runner.cpp ../email/sleela_email.c ../email/sleela_email.h
	$(CXX) $(CXXFLAGS) -I../email bodi_xml_runner.cpp ../email/sleela_email.c $(OPENSSL_LIBS) -o $@

clean:
	rm -f bodi-xml-runner
