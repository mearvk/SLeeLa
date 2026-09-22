# BODI XML Projects

BODI XML adds a declarative XML representation to SLeeLa's existing BODI witness model.

Executable project families:
- Science: math and physics operations are currently executable through an allow-listed dispatcher.
- Email / SMTP: the existing Email Facade can be configured from XML.

SMTP credentials are read from environment variables and are not stored in XML.

Build:

    make

Run science projects:

    ./bodi-xml-runner examples/science-math.xml
    ./bodi-xml-runner examples/science-physics.xml

SMTP projects are dry-run by default. Actual network submission requires:

    ./bodi-xml-runner examples/email-smtp.xml --send-email

Each executed operation emits a BODI witness record. XML cannot invoke shell commands or arbitrary native functions.
