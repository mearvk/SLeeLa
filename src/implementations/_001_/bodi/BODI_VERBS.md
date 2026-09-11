# Bodi Middle Verbs

## Purpose

Bodi uses a middle layer of verbs between a programmer's object reference and the underlying transport or implementation. The middle verb names the intended change to an addressed system while the Witness layer records what was requested, against which reference, in which sequence, by which starter, and with which datum.

The vocabulary is deliberately modest. A verb should describe one meaningful state transition rather than become a general-purpose command language.

## Canonical Positive / Negative Pairs

| Positive | Negative | Meaning |
|---|---|---|
| `install` | `uninstall` | Place or remove a named capability in a system. |
| `connect` | `disconnect` | Establish or end a system relationship. |
| `push` | `pull` | Publish or retrieve a datum/object reference. |
| `enable` | `disable` | Permit or prevent an addressed capability. |
| `start` | `stop` | Begin or end an active service sequence. |
| `open` | `close` | Open or close an explicitly addressed resource. |
| `attach` | `detach` | Join or remove an object from a system context. |
| `bind` | `unbind` | Associate or remove a name from an object. |
| `register` | `unregister` | Publish or withdraw a service identity. |
| `mount` | `unmount` | Place or remove a resource from the active namespace. |
| `activate` | `deactivate` | Make or cease making a capability operational. |
| `admit` | `reject` | Accept or refuse an offered object or change. |
| `approve` | `revoke` | Grant or withdraw an explicit authorization. |
| `commit` | `rollback` | Make a witnessed change durable or reverse it. |
| `publish` | `withdraw` | Make a descriptor/change discoverable or remove it. |
| `expose` | `conceal` | Make an interface discoverable or hide it. |
| `observe` | `ignore` | Record or decline a witness observation. |
| `propagate` | `contain` | Carry a change onward or stop it at the current boundary. |

This gives **36 canonical words** arranged as **18 positive/negative pairs**.

## Semantic Rule

A Bodi middle verb should be interpreted as a request against an already-addressed system, not as an unrestricted imperative over the machine.

```text
system
  -> propagation
      -> middle verb
          -> datum
              -> witness
                  -> resulting object change
```

For example:

```java
Bodi.system("xxx")
    .propagative("001")
    .install("network_witness");
```

The negative counterpart is:

```java
Bodi.system("xxx")
    .propagative("001")
    .uninstall("network_witness");
```

The important distinction is that `install` and `uninstall` describe the **change**, while the Bodi Witness describes the **fact that the change was addressed and observed**.

## Why These Verbs

The vocabulary covers five useful dimensions:

1. **Lifecycle** — `start/stop`, `open/close`, `activate/deactivate`.
2. **Relationship** — `connect/disconnect`, `attach/detach`, `bind/unbind`, `mount/unmount`.
3. **Publication and discovery** — `push/pull`, `register/unregister`, `publish/withdraw`, `expose/conceal`.
4. **Authority and disposition** — `enable/disable`, `admit/reject`, `approve/revoke`, `commit/rollback`.
5. **Witness propagation** — `observe/ignore`, `propagate/contain`.

`install/uninstall` remains the preferred capability-changing pair. `propagate/contain` is the preferred pair for describing whether a witnessed change crosses a system boundary.

## XML Representation

The same operation can be carried over the agreed Bodi XML structure:

```xml
<bodi xmlns="urn:sleela:bodi:1"
      system="xxx"
      sequence="001"
      method="install"
      starter="network"
      man="operator">
    <datum>network_witness</datum>
</bodi>
```

The negative operation changes only the middle verb:

```xml
<bodi xmlns="urn:sleela:bodi:1"
      system="xxx"
      sequence="001"
      method="uninstall"
      starter="network"
      man="operator">
    <datum>network_witness</datum>
</bodi>
```

## Inversion Is Explicit

The inverse of a verb is not inferred from arbitrary English. Bodi maintains a closed vocabulary so that a system can determine whether a requested negative operation has a defined positive counterpart.

Examples:

```text
inverse(install)    = uninstall
inverse(connect)    = disconnect
inverse(enable)     = disable
inverse(commit)     = rollback
inverse(propagate)  = contain
```

This permits an implementation to reject an unknown or semantically unsupported middle verb before it reaches the target object.

## Design Principle

Bodi should remain a **witness layer to object change**, not become a second programming language. The programmer identifies the system and sequence; the middle verb states the intended transition; the datum identifies what is being acted upon; the backend performs the appropriate local or network operation; and the Witness records the resulting addressable event.
