# SLeeLa XML Examples

This directory contains clear, runnable examples for the SLeeLa BODI XML project system. The examples are also evidence artifacts: each XML project has a corresponding expected BODI witness output where the result is deterministic and does not require external network access.

## Example families

| Example | Purpose | Expected evidence |
|---|---|---|
| 01 | Mathematics | BODI witness from the math dispatcher |
| 02 | Physics | BODI witness from the physics dispatcher |
| 03 | Chemistry | BODI witness from the chemistry dispatcher |
| 04 | Mixed science | Witnesses from multiple science disciplines |\n| 11 | Data analytics | `11-data-analytics.expected.txt` |
| 05 | SMTP email | Explicit network submission example |
| 06 | SMTP dry run | Safe SMTP configuration validation |
| 07 | HTTP-style POST | `07-post.expected.txt` |
| 08 | Listener declaration | `08-listener.expected.txt` |
| 09 | Router declaration | `09-router.expected.txt` |

## Running

Build the BODI runner:

    cd ../api/bodi
    make

Run the examples:

    ../api/bodi/bodi-xml-runner 01-math.xml
    ../api/bodi/bodi-xml-runner 02-physics.xml
    ../api/bodi/bodi-xml-runner 03-chemistry.xml
    ../api/bodi/bodi-xml-runner 04-mixed-science.xml
    ../api/bodi/bodi-xml-runner 06-email-dry-run.xml
    ../api/bodi/bodi-xml-runner 07-post.xml
    ../api/bodi/bodi-xml-runner 08-listener.xml
    ../api/bodi/bodi-xml-runner 09-router.xml\n    ../api/bodi/bodi-xml-runner 11-data-analytics.xml

SMTP transmission is intentionally explicit:

    ../api/bodi/bodi-xml-runner 05-email-smtp.xml --send-email

Credentials are referenced through environment variables rather than stored in XML.

## XML vocabulary

Every project starts with `<bodi version="1">` and a `<project>` declaration.

The current project kinds are:

- `science` — executable math, physics, chemistry, and data-analytics operations.
- `email` — SMTP configuration and message submission.
- `post` — HTTP-style POST representation.
- `listener` — listener bind/protocol/route declaration.
- `router` — deterministic method/path-to-target route declarations.

The native C vocabulary is under:

- `api/posting/`
- `api/listener/`
- `api/router/`

XML is declarative and allow-listed. It cannot invoke arbitrary shell commands or arbitrary native functions. The post, listener, and router examples validate and witness their declarations without opening sockets or transmitting network traffic.

## 10 — Apache/Tomcat Web Server Deployment

`10-webserver-deployment.md` shows how a web-aware application can hand its deployment artifact and health endpoint to the cross-platform `api/webserver/` module.
