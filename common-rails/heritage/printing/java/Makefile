JAVAC?=javac
all: check
check:
	$(JAVAC) -d . CommonRailsPrinting.java PrintContractTest.java
	java -ea -cp . PrintContractTest
clean:
	rm -rf com
	rm -f *.class
