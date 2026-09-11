# SLeeLa + Java Swing

## Purpose

`JavaSWING.md` defines the Java Swing integration path for SLeeLa applications. Swing remains a strictly Java desktop presentation layer while SLeeLa remains authoritative for business operations and computation.

The common SLeeLa Java connector gives Swing applications one integration contract across local process, RMI, and HTTP transports.

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
                Java Swing
```

The architectural rule is:

> **Swing presents; SLeeLa decides.**

Swing owns frames, panels, controls, event handling, and presentation. SLeeLa owns the business operation invoked by the application.

## Common Connector API

The common connector package is:

`com.mearvk.sleela.connector`

A Swing application should depend on the transport-neutral types:

- `SleelaJavaConnector` — common Java connector interface.
- `SleelaInvocation` — operation and argument envelope.
- `SleelaResult` — normalized success/error result.

The Swing layer can therefore use one call shape:

```java
SleelaResult result = connector.invoke("calculate", "42");
```

The underlying connector may be local process, RMI, or HTTP without changing the Swing action code.

## Local Process Integration

For a Swing application deployed beside SLeeLa, use the process connector.

```java
SleelaJavaConnector connector =
        new SleelaProcessConnector(executable, workingDirectory);

SleelaResult result = connector.invoke("calculate", "42");
```

This is a strong desktop model when SLeeLa is installed locally and process separation is desirable.

Advantages include:

- No network service is required.
- The Java application remains a conventional Swing program.
- SLeeLa remains a separate executable authority.
- Connector transport details stay outside the UI.

## RMI Integration

For a Swing client communicating with a SLeeLa service, use `SleelaRmiConnector`.

```java
SleelaJavaConnector connector =
        new SleelaRmiConnector(new SleelaRmiEndpoint(
                "127.0.0.1", 1099, "sleela"));

SleelaResult result = connector.invoke("calculate", "42");
```

RMI is particularly natural for a Java-to-Java integration boundary because the Swing application remains entirely Java while the remote implementation remains SLeeLa-backed.

Use this model for:

- Desktop clients and dedicated service processes.
- Multiple Swing clients.
- Controlled enterprise networks.
- Existing SLeeLa RMI services.

## HTTP Integration

A Swing application can use `SleelaHttpConnector` when the SLeeLa service is exposed through the common HTTP gateway.

```java
SleelaJavaConnector connector =
        new SleelaHttpConnector(URI.create(
                "http://127.0.0.1:8080/sleela"));

SleelaResult result = connector.invoke("calculate", "42");
```

HTTP allows Swing to participate in a broader web-oriented service architecture without putting browser or web protocol code into the business logic itself.

## Swing Event Handling

A Swing event handler should invoke SLeeLa through the connector rather than embedding SLeeLa protocol logic in the event listener.

```java
calculateButton.addActionListener(event -> {
    SleelaResult result = connector.invoke("calculate", inputField.getText());
    outputArea.setText(result.success()
            ? result.value()
            : "SLeeLa error: " + result.error());
});
```

For operations that can take time, the connector call should run outside the Swing Event Dispatch Thread.

## SwingWorker Pattern

Use `SwingWorker` for long-running SLeeLa operations.

```java
new SwingWorker<SleelaResult, Void>() {
    @Override
    protected SleelaResult doInBackground() throws Exception {
        return connector.invoke("calculate", inputField.getText());
    }

    @Override
    protected void done() {
        // Retrieve the result and update Swing controls on the EDT.
    }
}.execute();
```

This keeps the Swing user interface responsive while SLeeLa performs computation or a remote operation.

## Health and Lifecycle

The connector provides a separate health boundary so Swing applications can display connection state without turning health checks into arbitrary business calls.

For example, a desktop application may show:

```text
SLeeLa Service: Healthy
Transport: RMI
Service: sleela
```

Connector resources should be closed when the Java application shuts down.

```java
try (SleelaJavaConnector connector = ...) {
    // Swing application work
}
```

Closing a client connector does not imply that a remote SLeeLa service should be stopped.

## Error Handling

The connector normalizes ordinary operation results into `SleelaResult`. Swing should distinguish between:

1. Successful SLeeLa execution.
2. An application-level failure returned by SLeeLa.
3. A connector or transport failure.
4. A Swing presentation failure.

For example, a transport outage should normally produce a connection/status message, not be presented as though SLeeLa deliberately rejected the business operation.

## Security

RMI and HTTP endpoints must be protected according to the deployment environment. Reachability is not authentication.

Production Swing applications should use authenticated or otherwise restricted services, controlled network exposure, and narrowly defined operations. Avoid exposing arbitrary Java serialization, unrestricted remote methods, or arbitrary process execution through a connector endpoint.

## Web-Driven Swing Applications

Swing can also coexist with web-driven artifacts. A Java application can host an HTTP gateway while Swing provides a local operator or administrative interface.

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
          ^
          |
       Java Swing
```

The important point is that both presentation paths can use the same SLeeLa operation model without duplicating business logic.

## Relationship to JavaFX

Swing and JavaFX should use the same connector abstraction rather than creating separate SLeeLa protocols.

```text
                 SLeeLa
                    |
          SleelaJavaConnector
                    |
          +---------+---------+
          |                   |
       JavaFX              Swing
          |                   |
       Desktop             Desktop
```

This makes the SLeeLa integration reusable across generations of Java desktop UI technology.

## Design Principle

The connector layer deliberately separates:

- **Presentation:** Java Swing.
- **Integration:** `SleelaJavaConnector`.
- **Authority:** SLeeLa.

The result is a conventional Java desktop application that can interface with SLeeLa without becoming responsible for SLeeLa's business logic or execution model.
