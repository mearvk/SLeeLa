# SLeeLa Email Facade — SleelaDoc

## STARTTLS example

    send Email {
      smtp_host = "smtp.example.com"
      smtp_port = 587
      helo_name = "mail.example.com"
      local_bind_host = "192.0.2.10"
      tls_mode = starttls
      username = "mailer@example.com"
      password = "<credential-from-secret-store>"
      from = "mailer@example.com"
      to = "recipient.example.net"
      subject = "SLeeLa message"
      body = "Hello from the SLeeLa Email Facade."
    }

## Implicit TLS example

    send_from_tls("smtp.example.com", 465, "mail.example.com",
                  "mailer@example.com", "recipient@example.net",
                  "Secure message", "Hello over implicit TLS.")

Use authorized SMTP identities and interfaces. Do not commit production passwords to source control.
