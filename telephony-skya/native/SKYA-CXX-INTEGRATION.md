# Skya C/C++ Integration Guide

Skya is a module of SLeeLa, not a replacement runtime.

Production adapters should bind to existing SLeeLa domains for core runtime and threads, sockets and files, HTTP multiplexing, server/NAT facilities, security and certificates, media, and platform firewall adapters.

Do not introduce another VM, another general-purpose socket abstraction, or an independent firewall policy engine inside Skya. Translate SLeeLa types into telephony-specific records only at the adapter boundary.

The foundation still needs real HTTP/2 and HTTP/3 transport binding, media capture and codec adapters, certificate-quality extraction, NAT traversal, file-transfer framing, and OS firewall adapters before it should be described as a complete telephony implementation.
