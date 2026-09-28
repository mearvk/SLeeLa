# SLeeLa RMI

## SLeeLa as a brother to Java RMI

SLeeLa RMI is a peer integration with Java, not a Java imitation. Java supplies the mature RMI/JRMP transport, registry, proxy, and distributed-object machinery; SLeeLa supplies the business logic represented by the remote service.

The boundary is:

```text
Remote Java client
       |
       v
SleelaRmiClient
       |
       v
Java RMI / JRMP
       |
       v
SleelaRemote
       |
       v
SleelaRmiService
       |
       v
SleelaRuntime
       |
       +--------------------+
       |                    |
ProcessSleelaRuntime   JniSleelaRuntime
       |                    |
       +---------+----------+
                 |
                 v
             SLeeLa VM
```

The principle is simple: **Java transports; SLeeLa decides.**

## Complete backing package

The RMI implementation now includes the server lifecycle, client facade, endpoint model, remote contract, service adapter, and executable demonstrations:

```text
rmi/
  java/com/mearvk/sleela/rmi/
    SleelaRemote.java
    SleelaRmiEndpoint.java
    SleelaRmiService.java
    SleelaRmiServerHandle.java
    SleelaRmiServer.java
    SleelaRmiClient.java
    SleelaRmiClientDemo.java
    SleelaRmiServerDemo.java
  examples/
    server/RmiServer.sleela
    client/RmiClient.sleela
```

### Remote contract

`SleelaRemote` is the stable RMI contract. It exposes three deliberately small operations:

```java
String serviceName() throws RemoteException;
String invoke(String operation, String arguments) throws RemoteException;
String health() throws RemoteException;
```

The contract keeps transport concerns out of the SLeeLa language itself. Strings are used at the first boundary so that Java object serialization does not become the definition of SLeeLa semantics.

### Server adapter

`SleelaRmiService` extends `UnicastRemoteObject` and delegates `invoke()` directly into the configured `SleelaRuntime`. That runtime can be the process-backed implementation today or a native/JNI implementation later.

### Server lifecycle

`SleelaRmiServerHandle` owns the complete lifecycle:

1. Create the standard Java RMI registry.
2. Export the SLeeLa service.
3. Bind the service name.
4. Report health.
5. Unbind and unexport during shutdown.

This separates lifecycle management from the command-line bootstrap and makes the server usable from another Java host as a library.

### Server executable

```sh
java com.mearvk.sleela.rmi.SleelaRmiServer \
  SLeeLa /path/to/sleela /path/to/application 1099
```

The server keeps the process alive and installs a JVM shutdown hook so the RMI service and SLeeLa runtime are released together.

### Client facade

`SleelaRmiClient` resolves the registry entry and exposes a small client API:

```java
SleelaRmiEndpoint endpoint =
    new SleelaRmiEndpoint("127.0.0.1", 1099, "SLeeLa");

try (SleelaRmiClient client = new SleelaRmiClient(endpoint)) {
    client.health();
    client.serviceName();
    client.invoke("main", "");
}
```

`isHealthy()` provides a non-throwing liveness probe for administration and GUI code.

### Client executable

```sh
java com.mearvk.sleela.rmi.SleelaRmiClientDemo \
  127.0.0.1 1099 SLeeLa
```

The demo performs health, identity, and invocation calls through the actual remote proxy.

### End-to-end demo

`SleelaRmiServerDemo` starts a complete in-process RMI registry and service, connects through a separate client facade, performs a round trip, and then closes the service. It is useful for validating the Java-side RMI machinery independently of a particular SLeeLa executable.

## Runtime ownership

The Java RMI objects are transport adapters. The operation remains SLeeLa-owned:

```text
SleelaRmiClient
      |
      | JRMP
      v
SleelaRmiService
      |
      v
SleelaRuntime.call()
      |
      v
SLeeLa business logic
```

The current command-line server selects `ProcessSleelaRuntime`. The architecture also leaves a direct JNI/native path available through `JniSleelaRuntime`, allowing the RMI contract to remain stable while the runtime binding becomes more direct.

## Contract discipline

The first contract intentionally uses a bounded textual operation/argument pair. The next typed layer can map the SLeeLa core's value model into explicit RMI DTOs:

```text
SLeeLa value
    <-> explicit RMI DTO
    <-> Java value
```

That is preferable to making arbitrary Java serialization the semantic boundary of SLeeLa.

## Security and deployment

Java RMI should not be exposed as an unauthenticated public Internet service. Production deployments should:

- restrict registry and remote-object ports with network policy;
- bind only to intended interfaces;
- use appropriate authentication and protected transport;
- keep the remote contract narrow;
- avoid arbitrary Java object parameters;
- treat the SLeeLa operation set as an explicit capability boundary.

The current examples are intentionally simple local/service-network examples and should not be interpreted as a production security configuration.

## GUI relationship

A Swing or JavaFX application can use `SleelaRmiClient` as its remote business-logic connection while SLeeLa remains the authority for application behavior. Administrative and monitoring interfaces can use the same remote contract.

```text
Swing / JavaFX
      |
      v
SleelaRmiClient
      |
      v
Java RMI / JRMP
      |
      v
SleelaRmiService
      |
      v
SLeeLa
```

This preserves the broader SLeeLa architecture: **Java presents; SLeeLa decides.**
