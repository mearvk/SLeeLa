# Email Facade Security

## TLS

The facade uses OpenSSL TLS rather than obsolete SSL protocol versions. TLS 1.2 is the configured minimum. The peer certificate is verified against the OpenSSL trust store, with hostname or IP verification against smtp_host.

The terminology "SSL" may appear in deployments because SMTP services historically use names such as SMTPS. The implementation uses modern TLS.

## Credentials

AUTH PLAIN is permitted only when TLS is active. Credentials are held in process memory and are not persisted by the facade.

## Identity

smtp_host identifies the SMTP service. helo_name identifies the SMTP client in EHLO. local_bind_host selects the local source address when the operating system has that address.

The From address does not itself grant authorization to send mail. SMTP server policy, DNS, SPF, DKIM, DMARC, and provider authorization remain external controls.

## Input safety

CR/LF characters are rejected in EHLO, From, To, and Subject fields to prevent SMTP header injection. Message bodies are bounded.

Production applications should also apply provider-specific address, content, rate, and anti-abuse policies.
