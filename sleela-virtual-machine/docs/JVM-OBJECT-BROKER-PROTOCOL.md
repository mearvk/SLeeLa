# SLVM ↔ JVM Object Broker Protocol

The broker is the controlled boundary between native SLVM and a remote Java/JVM host. JavaFX GUI content may be hosted in the JVM while SLeeLa execution remains in SLVM.

Transport is neutral: TCP/TLS, Unix-domain sockets, named pipes, or another approved secure transport may implement the callbacks. The protocol uses a fixed binary frame followed by an exact payload length. SLJV (0x534C4A56) identifies the protocol; version 1 is the initial contract.

Message flow:
1. HELLO negotiates version, runtime identity, capabilities, and peer identity.
2. OBJECT_DECLARE assigns a stable broker object ID and class/type metadata.
3. OBJECT_CALL sends a method/function ID and serialized parameters.
4. OBJECT_RETURN returns status and serialized result.
5. GUI_CREATE requests a JavaFX-hosted GUI object.
6. GUI_EVENT carries UI events back to SLVM.
7. GUI_CLOSE closes the remote presentation.
8. OBJECT_RELEASE releases a proxy.
9. ERROR reports a structured broker error.
10. CLOSE terminates the session.

No Java object reference is passed directly into native memory. SLVM receives an opaque broker object ID.

Security requires encryption and peer identity. Preferred deployment is TLS 1.3 with mutual certificates or an equivalent platform-secure channel. Passwords are not protocol fields. A password, PSK, or private key is referenced through an external credential provider/OS secret store and is never written into broker frames, logs, object payloads, or observer records. The protocol delegates key agreement, encryption, certificate validation, rotation, and secure storage to an approved TLS/crypto provider.

GUI_CREATE contains a logical class/view identifier, properties, and capability declaration. The JVM owns JavaFX thread confinement and native GUI objects. SLVM owns application semantics and authorization. GUI events are subject to SLVM observer, security, memory, and capability policy.

Serialized kinds are scalar, string, bytes, list, map, class, GUI, and stream. Handles are session-scoped and bound to authenticated session identity and object IDs.

Copyright (c) Max Rupplin - MEARVK LLC - 2026