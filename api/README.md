# SLeeLa API

The API tree contains native C/C++ facades and SLeeLa mappings.

## Email Facade

The Email Facade is at api/email. It provides SMTP-aware submission using plain SMTP, STARTTLS, and implicit TLS, with OpenSSL certificate validation and optional AUTH PLAIN when TLS is active.

## Posting / Listener / Router API

The native C facades in `api/posting`, `api/listener`, and `api/router` define the basic vocabulary used by XML projects for POST representations, listener declarations, and deterministic method/path routing.

## BODI XML Projects

The BODI XML API is at api/bodi.

It provides a versioned XML project envelope, BODI witness records, executable Math and Physics discipline handlers, SMTP project configuration backed by the existing Email Facade, environment-variable references for SMTP credentials, XML schema documentation, deterministic examples, and dry-run-by-default SMTP execution.

See api/bodi/index.html and api/bodi/API.html.
