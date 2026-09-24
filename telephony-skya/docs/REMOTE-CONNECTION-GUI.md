# Skya Remote Connection GUI

Skya provides a second JavaFX surface for communicating with remote servers. The existing Client Monitor remains for local engine/circuit monitoring; this GUI is the endpoint-oriented client.

The remote GUI provides server host/port, room and protocol selection, connect/disconnect controls, listener output, and client message submission. Its runtime events use Guia™ operations such as CLIENT.CONNECT, SESSION.OPEN, LISTENER.START, LISTENER.RECEIVE, COMMAND.INVOKE, CLIENT.DISCONNECTED, and SESSION.CLOSED.

The current adapter uses a small TCP connection as the transport foundation. Selecting HTTP/2 or HTTP/3 records the intended protocol and sends the Skya connection declaration; it does not claim that a plain Java TCP socket implements HTTP/2 or HTTP/3. Production HTTP/2, HTTP/3/QUIC, TLS, NAT traversal, authentication and media transport remain native transport implementations.

Guia™ is the common GUI-to-client/listener contract; JavaFX is the presentation adapter.
