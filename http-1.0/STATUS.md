# HTTP 1.0 — Port Multiplexing Status

HTTP 1.0 establishes the common SLeeLa logical-port contract. Native transport endpoints remain separate from application routes. Because HTTP 1.0 has no HTTP/2 or HTTP/3 stream namespace, multiplexing is performed by the SLeeLa routing layer and the surrounding HTTP connection model.

Cross-version rule: native endpoint ≠ HTTP stream ≠ SLeeLa logical PORT. For HTTP 1.0 there is no HTTP stream, but the logical PORT remains independent of native socket numbering.
