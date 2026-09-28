# BODI XML Projects

BODI XML adds a declarative XML representation to SLeeLa's existing BODI witness model.

Executable project families:
- Science: math, physics, chemistry, and data-analytics operations are currently executable through an allow-listed dispatcher.
- Email / SMTP: the existing Email Facade can be configured from XML.
- Post: an HTTP-style POST representation can be validated from XML.
- Listener: an HTTP listener declaration can be validated from XML.
- Router: deterministic method/path route entries can be validated from XML.

SMTP credentials are read from environment variables and are not stored in XML.

Build:

    make

Run science projects, including the data-analytics shelf:

    ./bodi-xml-runner examples/science-math.xml
    ./bodi-xml-runner examples/science-physics.xml

SMTP projects are dry-run by default. Actual network submission requires:

    ./bodi-xml-runner examples/email-smtp.xml --send-email

Each executed operation emits a BODI witness record. XML cannot invoke shell commands or arbitrary native functions.

## Posts, listeners, and routers

The XML vocabulary also supports basic project declarations for:
- `kind="post"` with `<post target="/path" content-type="..." body="..."/>`
- `kind="listener"` with `<listener bind="127.0.0.1" port="8080" protocol="http" route="/"/>`
- `kind="router"` with one or more `<route method="POST" path="/path" target="/handler"/>` entries.

The C facades live under `api/posting`, `api/listener`, and `api/router`. The XML runner validates and witnesses these objects without implicitly opening sockets or transmitting data.
