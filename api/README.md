# SLeeLa API

The API tree contains native C/C++ facades and SLeeLa mappings.

## Email Facade

The Email Facade is at api/email.

It provides SMTP-aware submission using:
- SMTP over a plain socket;
- SMTP STARTTLS;
- SMTP implicit TLS;
- OpenSSL certificate validation for TLS endpoints;
- DNS-name or IP endpoint addressing;
- optional local source-IP binding;
- optional SMTP AUTH PLAIN when TLS is active;
- bounded message fields and diagnostics.

The SLeeLa layer maps the native transport as native.email.send.

See email/API.html, email/index.html, email/smtp.html, and examples/email/SleelaDoc.md.
