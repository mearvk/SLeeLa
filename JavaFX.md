# SLeeLa + JavaFX

## Purpose

`JavaFX.md` defines the JavaFX integration path for SLeeLa applications. JavaFX remains a strictly Java presentation layer while SLeeLa remains authoritative for business operations and computation.

The common connector system allows a JavaFX application to call SLeeLa without depending on a particular transport. The same JavaFX-facing code can use a local SLeeLa process, Java RMI, or HTTP.

## Architecture

```text
                 SLeeLa
                    |
          SLeeLa Java Connector
                    |
       +------------+------------+
       |            |            |
     Process       RMI          HTTP
       |            |            |
       +------------+------------+
                    |
               JavaFX UI
```

The architectural rule is:

> **JavaFX presents; SLeeLa decides.**

JavaFX owns windows, controls, scenes, event handling, and presentation. SLeeLa owns the business operation invoked by the UI.

## Common Connector API

The common connector package is:

`com.mearvk.sleela.connector`

The JavaFX layer should normally depend on:

- `SleelaJavaConnector` — transport-neutral Java connector interface.
- `SleelaInvocation` — operation and textual argument envelope.
- `SleelaResult` — normalized success/error result.

A JavaFX controller can therefore use the same call shape regardless of transport:

```java
SleelaResult result = connector.invoke("calculate", "42");
```

## Local Process Integration

For a JavaFX application deployed together with SLeeLa, the process connector is the simplest deployment model.

```java
SleelaJavaConnector connector =
        new SleelaProcessConnector(executable, workingDirectory);

SleelaResult result = connector.invoke("calculate", "42");
```

The JavaFX application owns the desktop lifecycle. The SLeeLa executable performs the requested operation.

This model is useful when:

- SLeeLa is installed on the same machine.
- No network service is required.
- The UI and runtime should have separate process boundaries.
- A desktop application should remain independent of SLeeLa implementation details.

## RMI Integration

For a JavaFX UI communicating with a separately running SLeeLa service, use `SleelaRmiConnector`.

```java
SleelaJavaConnector connector =
        new SleelaRmiConnector(new SleelaRmiEndpoint(
                "127.0.0.1", 1099, "sleela"));

SleelaResult result = connector.invoke("calculate", "42");
```

Java RMI provides the Java-native remote object boundary. SLeeLa remains the authority behind the remote service.

This is appropriate for:

- Desktop clients and service processes.
- Multiple JavaFX clients.
- Java-to-SLeeLa applications on a controlled network.
- Existing SLeeLa RMI deployments.

## HTTP Integration

A JavaFX application can also use `SleelaHttpConnector` when SLeeLa operations are exposed through the HTTP gateway.

```java
SleelaJavaConnector connector =
        new SleelaHttpConnector(URI.create(
                "http://127.0.0.1:8080/sleela"));

SleelaResult result = connector.invoke("calculate", "42");
```

HTTP is useful when the JavaFX application must communicate with a service that is already part of a web-oriented deployment.

## Controller Pattern

A JavaFX controller should keep connector calls behind application actions rather than embedding SLeeLa protocol details throughout the UI.

```java
public void calculate() {
    SleelaResult result = connector.invoke("calculate", inputField.getText());
    outputLabel.setText(result.success()
            ? result.value()
            : "SLeeLa error: " + result.error());
}
```

The controller knows that it is invoking an operation, but it does not need to know whether the operation is implemented through a local process, RMI, or HTTP.

## Threading

Remote or process-backed calls should not block the JavaFX Application Thread. Use JavaFX `Task`, `Service`, or another background execution mechanism for operations that may take meaningful time.

```text
JavaFX Application Thread
        |
        +--> background Task
                 |
                 +--> SleelaJavaConnector
                          |
                          +--> SLeeLa
```

UI updates should return to the JavaFX Application Thread after the operation completes.

## Health and Lifecycle

The common connector exposes a health operation independently from business calls. A JavaFX application can use health checks to update connection indicators without inventing a business operation for connectivity.

Connector ownership should follow application ownership:

```java
try (SleelaJavaConnector connector = ...) {
    // JavaFX application work
}
```

The connector's `close()` method should release resources owned by that connector. A client-side RMI lookup does not shut down the remote service.

## Error Boundary

JavaFX should receive normalized `SleelaResult` values rather than depending on transport-specific exceptions for ordinary business failures.

Transport failures remain exceptional conditions and should be handled at the application boundary. The UI should distinguish:

1. A successful SLeeLa operation.
2. A valid SLeeLa operation that returned an application error.
3. A connector/transport failure.
4. A local JavaFX presentation failure.

This keeps the presentation layer from confusing a network failure with a business decision.

## Security

A JavaFX application must not assume that RMI or HTTP endpoints are trustworthy merely because they are reachable. Production deployments should use authenticated and appropriately restricted services, protected network boundaries, and narrowly defined SLeeLa operations.

Do not expose arbitrary Java object deserialization or unrestricted Java execution through a SLeeLa connector.

## Web-Driven JavaFX

JavaFX can also act as the Java host for a web-oriented artifact. In that arrangement, the Java application may host the HTTP gateway while JavaFX supplies an administrative or operator interface.

```text
Browser / Web Artifact
          |
         HTTP
          |
   Java HTTP Gateway
          |
 SLeeLa Java Connector
          |
        SLeeLa
```

This permits a browser-facing artifact and a native JavaFX interface to use the same SLeeLa operation model.

## Design Principle

The connector layer deliberately separates three concerns:

- **Presentation:** JavaFX.
- **Integration:** the Java connector.
- **Authority:** SLeeLa.

This makes JavaFX a clean, strictly-Java presentation system without requiring the UI to become a SLeeLa implementation.
