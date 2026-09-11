# SLeeLa RMI

## SLeeLa as a brother to Java RMI

SLeeLa RMI is designed as a **peer integration with Java**, not as a Java imitation. Java supplies the mature RMI transport, registry, proxy, lifecycle, and distributed-object machinery; SLeeLa supplies the business logic that the remote object represents.

The intended relationship is:

```text
                    RMI
                     |
          +----------+----------+
          |                     |
        Java                  SLeeLa
     transport              business logic
     / registry              / Wrapper™
          |                     |
          +----------+----------+
                     |
              remote contract
```

In other words, SLeeLa is Java's **brother with its own flavor**: the two runtimes can meet at a strongly defined remote contract without requiring SLeeLa to become Java.

## Package

```text
rmi/
  java/com/mearvk/sleela/rmi/
    SleelaRemote.java
    SleelaRmiService.java
    SleelaRmiServer.java
    SleelaRmiClient.java
    SleelaRmiClientDemo.java
  examples/
    server/RmiServer.sleela
    client/RmiClient.sleela
```

## Why Java RMI

The first implementation intentionally uses the established Java RMI model:

- `Remote` defines the remote contract.
- `RemoteException` makes transport failure explicit.
- `UnicastRemoteObject` exports the server object.
- `LocateRegistry` provides the standard registry mechanism.
- `Registry.rebind()` publishes the service.
- `Registry.lookup()` obtains the client-side remote proxy.
- JRMP remains the transport selected by the standard Java RMI stack.

This gives SLeeLa a native Java-side RMI presence without inventing another RPC protocol prematurely.

## SLeeLa ownership model

The Java object is a **transport adapter**. The operation is still delegated to SLeeLa:

```text
Remote client
     |
     v
SleelaRemote.invoke()
     |
     v
SleelaRmiService
     |
     v
SleelaRuntime
     |
     v
SLeeLa Wrapper™
     |
     v
C/C++ VM / slcore_exchange()
```

The current service uses the existing `ProcessSleelaRuntime`, which gives us a working process boundary today. The same `SleelaRmiService` can later be constructed around `JniSleelaRuntime` or another direct VM binding without changing the remote contract.

## Server

Example:

```sh
java com.mearvk.sleela.rmi.SleelaRmiServer \
  SLeeLa /path/to/sleela /path/to/application 1099
```

The server creates the registry, exports the SLeeLa service, and binds it under the supplied name.

## Client

```sh
java com.mearvk.sleela.rmi.SleelaRmiClientDemo \
  127.0.0.1 1099 SLeeLa
```

The client performs a health check, obtains the service name, and invokes the SLeeLa `main` operation through the remote contract.

## Contract discipline

The initial contract deliberately uses strings at the outer boundary:

```java
String invoke(String operation, String arguments)
```

This is intentional. It avoids coupling the first SLeeLa RMI contract to Java's object serialization model. A future typed contract can map the SLeeLa core's value model directly. The current native core exposes `SLValue` with `null`, integer, double, boolean, and string variants, and exposes `slcore_exchange()` as its stable execution boundary.

A future `SleelaRemoteValue` family can therefore provide:

```text
SLeeLa SLValue  <->  RMI value DTO  <->  Java value
```

rather than making Java serialization the definition of SLeeLa semantics.

## Security and deployment

Java RMI is powerful but should not be treated as an unauthenticated Internet protocol. Production deployments should restrict registry and remote-object ports with network policy, bind only to intended interfaces, use authenticated/secured transport where required, and keep the remote contract narrow.

The SLeeLa RMI layer should also avoid accepting arbitrary Java classes as remote parameters. Explicit SLeeLa DTOs and a bounded value model are preferred over unconstrained Java serialization.

## Future native path

The long-term SLeeLa arrangement is:

```text
       Java RMI client
              |
              v
       SleelaRemote
              |
              v
      SleelaRmiService
              |
       +------+------+
       |             |
 Process runtime   JNI/native runtime
       |             |
       +------+------+
              |
              v
       slcore_exchange()
              |
              v
          SLeeLa VM
```

Thus the RMI contract remains stable while the implementation moves from a safe/simple process boundary toward a direct native runtime boundary.

## Relationship to the GUI library

RMI and GUI are complementary. A JavaFX or Swing application can act as an RMI client while SLeeLa remains the remote business-logic server. Conversely, a SLeeLa service can use the same contract while a Java host supplies administrative or monitoring interfaces.

```text
JavaFX / Swing
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
SLeeLa business logic
```

This preserves the division established by `gui/`: **Java presents; SLeeLa decides.**
