# SLeeLa ↔ Java Connector System

The SLeeLa connector system provides one common Java integration contract for applications that need to call SLeeLa without depending on a particular transport.

## Design

```text
                         SLeeLa
                            │
                  Java Connector Contract
                            │
          ┌─────────────────┼─────────────────┐
          │                 │                 │
       Process             RMI               HTTP
          │                 │                 │
      Java host         Java desktop       Web host
      / local app       Swing / JavaFX     / browser
```

The Java side presents the integration surface. SLeeLa remains authoritative for the business operation.

## Common API

`SleelaJavaConnector` is the transport-neutral interface. A Java program can use:

```java
SleelaResult result = connector.invoke("calculate", "input-data");
```

The same call can be backed by a local SLeeLa executable, Java RMI, or HTTP.

### Core types

- `SleelaJavaConnector` — common Java-side connector contract.
- `SleelaInvocation` — immutable operation request.
- `SleelaResult` — success/error result independent of transport.

## Local process

`SleelaProcessConnector` launches the existing SLeeLa executable through `ProcessSleelaRuntime`.

This is appropriate when the Java application and SLeeLa runtime live on the same machine.

## RMI

`SleelaRmiConnector` adapts the existing SLeeLa RMI service and client. It lets a strictly Java application use the same connector API while SLeeLa executes the underlying operation.

RMI is useful for Java-to-Java environments such as desktop applications and controlled enterprise networks. It should not be exposed as an unauthenticated public Internet service.

## HTTP and web-driven artifacts

`SleelaHttpConnector` uses the JDK `HttpClient` and talks to `SleelaHttpGateway`.

The gateway provides:

- `GET /sleela/health`
- `POST /sleela/invoke?operation=<name>`

The POST body is the operation's argument string and the response is UTF-8 text. This deliberately avoids Java serialization and external JSON dependencies.

For browser applications, the gateway can optionally emit CORS headers through its `allowOrigin` constructor argument. A production deployment should use a narrowly scoped origin and put authentication, TLS, rate limiting, and request-size controls in front of the gateway.

## Why this is common

A Swing UI, JavaFX application, servlet/web application, or other strictly Java program does not need to know whether SLeeLa is local, remote through RMI, or behind HTTP. The application depends on `SleelaJavaConnector`; deployment selects the adapter.

This creates a stable boundary:

```text
Java application → SleelaJavaConnector → transport adapter → SLeeLa
```

## Security boundary

The connector is an integration boundary, not an authorization system. Applications should:

1. Validate operations before exposing them to users or browsers.
2. Authenticate and authorize remote callers.
3. Restrict RMI registry and exported object ports.
4. Use HTTPS/TLS for remote HTTP deployments.
5. Apply request-size, timeout, and rate limits.
6. Avoid exposing arbitrary Java object serialization to SLeeLa callers.
7. Keep SLeeLa responsible for the authoritative business operation.

## Module note

The HTTP gateway uses the JDK `jdk.httpserver` module. Projects using the Java module system should include that module when compiling/running the gateway.

## Future extension

The textual connector contract is intentionally small. A future typed `SLValue` protocol can be added without changing the role of `SleelaJavaConnector`; only the invocation/result encoding needs to evolve.
