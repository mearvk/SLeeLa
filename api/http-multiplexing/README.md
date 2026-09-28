# SLeeLa HTTP Multiplexing and Download API

This API exposes the common HTTP design as source code.

Port namespace: exactly 10^48 logical values, represented by 20-byte big-endian data. A logical PORT is separate from native TCP/UDP ports and HTTP stream identifiers.

HTTP 1.0 uses request/connection routing; HTTP 2.0/2.1 uses HTTP/2 streams; HTTP 3.0 uses QUIC/HTTP/3 streams.

Files larger than 50 MB select resumable DOWNLOAD mode. Resume state contains SESSION-ID, DATETIME, FILE-ID, FILE-NAME, INDEX, OFFSET, and TOTAL-SIZE. FILE-ID identifies the transfer and INDEX/OFFSET identifies the resume position.

The API is a connector surface: applications can use the types and functions without becoming a SLeeLa runtime.
