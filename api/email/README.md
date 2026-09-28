# SLeeLa Email Facade

SMTP-aware native C API with a SLeeLa mapping.

Supported transport modes:
- NONE: unencrypted SMTP.
- STARTTLS: SMTP upgraded to TLS 1.2+.
- IMPLICIT: TLS before SMTP traffic, commonly port 465.

The SMTP server may be a DNS name or IP address. local_bind_host may select the local source address, while helo_name controls EHLO identity.

Credentials are used only in memory by this facade. Production applications should obtain them from a secret store or protected configuration rather than source control.
