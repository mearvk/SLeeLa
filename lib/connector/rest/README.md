# HTTP/REST connector

Provides HTTP GET and JSON POST via libcurl, with connection/total timeouts, TLS certificate and hostname verification, and redirects disabled. Responses are caller-owned until `sleela_http_response_free`. Transport success does not imply successful HTTP status; check `status`. Do not place credentials in URLs. This adapter does not yet expose arbitrary headers, OAuth, streaming, or retry policies. Build with `make -C lib/connector/rest` after installing libcurl development files.
