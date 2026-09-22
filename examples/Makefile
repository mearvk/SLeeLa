RUNNER ?= ../api/bodi/bodi-xml-runner

.PHONY: help math physics chemistry mixed email dry-run

help:
	@echo "make math|physics|chemistry|mixed|email|dry-run"

math:
	$(RUNNER) 01-math.xml

physics:
	$(RUNNER) 02-physics.xml

chemistry:
	$(RUNNER) 03-chemistry.xml

mixed:
	$(RUNNER) 04-mixed-science.xml

email:
	$(RUNNER) 05-email-smtp.xml --send-email

dry-run:
	$(RUNNER) 06-email-dry-run.xml
