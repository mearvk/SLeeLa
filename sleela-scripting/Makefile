.PHONY: all test check clean

all: check

check:
	@echo "Sleela Script source/specification check"
	@test -f README.md
	@test -f LANGUAGE.md
	@test -f GRAMMAR.md
	@test -f RUNTIME.md
	@test -f HOST-API.md
	@test -f SECURITY.md
	@test -f CONFIGURATION.md
	@test -f scripting.conf
	@test -f examples/hello.sleela-script
	@test -f tests/basic.sleela-script
	@echo "Sleela Script files: PASS"

test: check

clean:
	@echo "No generated scripting artifacts to remove."
