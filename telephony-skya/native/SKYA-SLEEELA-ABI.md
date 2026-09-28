# Skya / SLeeLa ABI Contract

SLeeLa owns the process runtime and platform resources. Skya owns telephony session state and room semantics. Callers own input strings for the duration of each call; the engine owns returned peer data and status storage. skya_destroy is terminal.

Roles are SKYA_CLIENT, SKYA_SERVER, and SKYA_BOTH.

Transport selection uses http_version=2 for HTTP/2 and http_version=3 for HTTP/3. The ABI records policy; the production wire transport is supplied by the SLeeLa HTTP/multiplexing implementation.

The engine protects mutable room/peer state internally. Zero is success; negative values are local API errors.
