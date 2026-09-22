# SLeeLa Email Facade API Example

This example describes an authorized SMTP submission using STARTTLS.

    module Example.Email

    email = Email {
      smtp_host = "smtp.example.com"
      smtp_port = 587
      helo_name = "mail.example.com"
      local_bind_host = "192.0.2.10"
      tls_mode = starttls
      username = "mailer@example.com"
      password = "<secret-store-reference>"
      from = "mailer@example.com"
      to = "recipient@example.net"
      subject = "SLeeLa API test"
      body = "This is a SLeeLa SMTP submission."

    native.email.send(email)

The local bind address must belong to the host running the program. The SMTP server and sender identity must be authorized by the operator and provider.
