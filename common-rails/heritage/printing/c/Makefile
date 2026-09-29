CC?=cc
CFLAGS?=-std=c11 -Wall -Wextra -Werror -I.
CORE=PrintLayout.c PrintField.c PrintLine.c PrintState.c PrintProgress.c PrintComponent.c PrintRenderer.c PrintWriter.c PrintFormatter.c
all: heritage-print-test
heritage-print-test: $(CORE) PrintContractTest.c
	$(CC) $(CFLAGS) -o $@ $(CORE) PrintContractTest.c
check: heritage-print-test
	./heritage-print-test
clean:
	rm -f heritage-print-test
