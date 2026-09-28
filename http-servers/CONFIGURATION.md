# HTTP Server Configuration

## Scope

This document records the configuration boundary for the three SLeeLa HTTP
server grades.

| Grade | Transport | Default endpoint | Primary configuration |
|---|---|---|---|
| 1 | HTTP/1.x over TCP | TCP 8080 | command-line options |
| 2 | HTTP/2 over TCP | TCP 8081 | command-line options |
| 3 | HTTP/3 over QUIC/UDP | UDP 8082 | command-line options plus QUIC TLS credentials |

The standalone HTTP server programs currently take their server-specific
settings from command-line arguments. They do **not** silently read or mutate
the general `config/sleela.properties.example` file.

## Grade 3 configuration

HTTP/3 requires:

- `--addr ADDRESS`
- `--port PORT`
- `--key PRIVATE_KEY_FILE`
- `--cert CERTIFICATE_FILE`
- optional `--backend PATH`

Example:

    sleela http-server 3 --addr 0.0.0.0 --port 8082 \
      --key /etc/sleela/tls/server.key \
      --cert /etc/sleela/tls/server.crt

The default backend name is `wsslserver`. A deployment may provide an explicit
backend path when the executable is not on `PATH`.

### TLS material

Private keys and certificates are deployment inputs, not repository source
files. They should remain outside Git, with filesystem permissions appropriate
to the service account. The repository does not provide a default private key
or certificate.

## Future configuration-file integration

A future SLeeLa HTTP service configuration file may describe listener,
certificate, logging, document-root, resource-limit, and backend settings.
When such a file is introduced, its schema should be versioned and its values
must not override explicit security-sensitive command-line arguments without
a documented precedence rule.

For now, deployment automation should treat the command line as the
authoritative HTTP server configuration interface.
