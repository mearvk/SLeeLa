# SLeeLa GUI Protocol™ — Guia™ 1.0

**Protocol name:** Guia™  
**Specification:** SLeeLa GUI Protocol™ 1.0  
**Owner:** MEARVK LLC  
**Purpose:** Language-neutral GUI-to-SLeeLa client/listener protocol.

## 1. Scope

Guia™ defines the names, objects, functions, states, events, transitions, bindings, and listener contract used when a GUI communicates with a SLeeLa client/runtime.

Architecture:

```
BODI/XML
   │
   ▼
Guia™ protocol model
   │
   ├── JavaFX adapter
   ├── Native GUI adapter
   └── Other GUI adapters
   │
   ▼
SLeeLa GUI Client
   │
   ▼
SLeeLa Listener / Runtime / Circuit
```

BODI describes the interface. Guia™ defines its executable interaction contract. SLeeLa remains authoritative for application and circuit behavior.

## 2. Naming model

Every Guia™ object has a stable `id`, a `type`, an optional `parent`, and a lifecycle state.

Reserved object namespaces:

- `gui.*` — application GUI
- `window.*` — windows
- `view.*` — views/panels
- `control.*` — interactive controls
- `layout.*` — layout containers
- `menu.*` — menus and menu items
- `dialog.*` — dialogs
- `data.*` — models and bindings
- `event.*` — events
- `command.*` — commands
- `client.*` — SLeeLa GUI client
- `listener.*` — listener endpoints
- `circuit.*` — SLeeLa circuits
- `monitor.*` — monitoring streams
- `session.*` — GUI/client sessions
- `error.*` — errors
- `security.*` — authorization/security context

Names are case-sensitive. Implementations MUST NOT reuse an active object ID.

## 3. Core objects

### 3.1 GuiApplication

Represents the GUI process/application.

Fields:

- `id`
- `name`
- `version`
- `protocol`
- `state`
- `capabilities`
- `clientId`

### 3.2 GuiClient

Represents the SLeeLa-facing GUI client.

Fields:

- `id`
- `applicationId`
- `state`
- `circuitId`
- `listenerId`
- `sessionId`
- `capabilities`

### 3.3 GuiListener

Represents the event/message listener receiving SLeeLa events.

Fields:

- `id`
- `state`
- `endpoint`
- `protocolVersion`
- `subscriptions`
- `lastSequence`

### 3.4 Circuit

Represents a loaded SLeeLa circuit.

Fields:

- `id`
- `path`
- `name`
- `state`
- `permissions`
- `inputs`
- `outputs`

### 3.5 Session

Represents one GUI-to-client interaction session.

Fields:

- `id`
- `clientId`
- `listenerId`
- `state`
- `createdAt`
- `sequence`
- `securityContext`

### 3.6 Control

Base GUI control.

Fields:

- `id`
- `type`
- `parent`
- `visible`
- `enabled`
- `value`
- `properties`
- `bindings`

Supported reference types include:

`button`, `label`, `text`, `password`, `checkbox`, `radio`, `select`, `list`, `table`, `tree`, `slider`, `progress`, `image`, `canvas`, `web`, `audio`, `video`, `status`, `log`.

### 3.7 DataModel

Represents GUI-visible application data.

Fields:

- `id`
- `schema`
- `value`
- `version`
- `readOnly`
- `source`

### 3.8 Command

Represents an operation callable by the GUI.

Fields:

- `id`
- `name`
- `arguments`
- `permissions`
- `timeout`
- `async`
- `state`

## 4. GUI functions

### Lifecycle

```GUI.CREATE(application)
GUI.INIT(application)
GUI.SHOW(gui)
GUI.HIDE(gui)
GUI.PAUSE(gui)
GUI.RESUME(gui)
GUI.UPDATE(gui)
GUI.CLOSE(gui)
GUI.DESTROY(gui)
```

### Window/view functions

```
WINDOW.CREATE
WINDOW.SHOW
WINDOW.HIDE
WINDOW.FOCUS
WINDOW.RESIZE
WINDOW.MOVE
WINDOW.MINIMIZE
WINDOW.MAXIMIZE
WINDOW.RESTORE
WINDOW.CLOSE

VIEW.CREATE
VIEW.SHOW
VIEW.HIDE
VIEW.ACTIVATE
VIEW.DEACTIVATE
VIEW.DESTROY
```

### Control functions

```
CONTROL.CREATE
CONTROL.DESTROY
CONTROL.SHOW
CONTROL.HIDE
CONTROL.ENABLE
CONTROL.DISABLE
CONTROL.FOCUS
CONTROL.BLUR
CONTROL.GET
CONTROL.SET
CONTROL.RESET
CONTROL.VALIDATE
CONTROL.REFRESH
```

### Data functions

```
DATA.CREATE
DATA.GET
DATA.SET
DATA.UPDATE
DATA.DELETE
DATA.RESET
DATA.SUBSCRIBE
DATA.UNSUBSCRIBE
DATA.SNAPSHOT
DATA.COMMIT
DATA.ROLLBACK
```

### Event functions

```
EVENT.BIND
EVENT.UNBIND
EVENT.EMIT
EVENT.CANCEL
EVENT.PAUSE
EVENT.RESUME
EVENT.SUBSCRIBE
EVENT.UNSUBSCRIBE
```

### Command functions

```
COMMAND.REGISTER
COMMAND.UNREGISTER
COMMAND.ENABLE
COMMAND.DISABLE
COMMAND.INVOKE
COMMAND.CANCEL
COMMAND.STATUS
COMMAND.RESULT
COMMAND.ERROR
```

### Client functions

```
CLIENT.CREATE
CLIENT.CONNECT
CLIENT.DISCONNECT
CLIENT.RECONNECT
CLIENT.PING
CLIENT.PONG
CLIENT.STATUS
CLIENT.CAPABILITIES
CLIENT.SHUTDOWN
```

### Circuit functions

```
CIRCUIT.LOAD
CIRCUIT.UNLOAD
CIRCUIT.START
CIRCUIT.STOP
CIRCUIT.PAUSE
CIRCUIT.RESUME
CIRCUIT.RELOAD
CIRCUIT.STATUS
CIRCUIT.INPUT
CIRCUIT.OUTPUT
```

### Listener functions

```
LISTENER.CREATE
LISTENER.START
LISTENER.STOP
LISTENER.PAUSE
LISTENER.RESUME
LISTENER.SUBSCRIBE
LISTENER.UNSUBSCRIBE
LISTENER.RECEIVE
LISTENER.ACK
LISTENER.NACK
LISTENER.RESET
LISTENER.DESTROY
```

### Session functions

```
SESSION.CREATE
SESSION.OPEN
SESSION.AUTHENTICATE
SESSION.RENEW
SESSION.SUSPEND
SESSION.RESUME
SESSION.CLOSE
SESSION.EXPIRE
```

### Monitoring functions

```
MONITOR.SUBSCRIBE
MONITOR.UNSUBSCRIBE
MONITOR.STATUS
MONITOR.METRIC
MONITOR.LOG
MONITOR.EVENT
MONITOR.ERROR
MONITOR.SNAPSHOT
```

## 5. Event names

Standard Guia™ events:

```
APP.START
APP.READY
APP.STOP
APP.ERROR

WINDOW.OPEN
WINDOW.ACTIVATE
WINDOW.DEACTIVATE
WINDOW.RESIZE
WINDOW.CLOSE

CONTROL.CREATED
CONTROL.CHANGED
CONTROL.FOCUSED
CONTROL.BLURRED
CONTROL.ENABLED
CONTROL.DISABLED
CONTROL.VALIDATED
CONTROL.ERROR

INPUT.CHANGED
INPUT.SUBMITTED
INPUT.CANCELLED

COMMAND.REQUESTED
COMMAND.STARTED
COMMAND.PROGRESS
COMMAND.COMPLETED
COMMAND.CANCELLED
COMMAND.FAILED

CLIENT.CONNECTING
CLIENT.CONNECTED
CLIENT.DISCONNECTED
CLIENT.RECONNECTING
CLIENT.READY
CLIENT.ERROR

CIRCUIT.LOADING
CIRCUIT.LOADED
CIRCUIT.STARTING
CIRCUIT.STARTED
CIRCUIT.PAUSED
CIRCUIT.RESUMED
CIRCUIT.STOPPING
CIRCUIT.STOPPED
CIRCUIT.FAILED

LISTENER.STARTING
LISTENER.READY
LISTENER.MESSAGE
LISTENER.ACKNOWLEDGED
LISTENER.REJECTED
LISTENER.STOPPING
LISTENER.STOPPED
LISTENER.ERROR

SESSION.OPENING
SESSION.OPEN
SESSION.AUTHENTICATING
SESSION.AUTHENTICATED
SESSION.SUSPENDED
SESSION.CLOSED
SESSION.EXPIRED

DATA.CREATED
DATA.CHANGED
DATA.COMMITTED
DATA.ROLLED_BACK
DATA.INVALID

MONITOR.STATUS
MONITOR.METRIC
MONITOR.LOG
MONITOR.ERROR
```

## 6. State machines

### GUI

```
CREATED → INITIALIZING → READY → ACTIVE
ACTIVE → PAUSED → ACTIVE
ACTIVE → CLOSING → CLOSED
INITIALIZING → ERROR
ACTIVE → ERROR
```

### Client

```
CREATED → CONNECTING → CONNECTED → READY
CONNECTING → FAILED
CONNECTED → DISCONNECTED
DISCONNECTED → RECONNECTING → CONNECTED
READY → SHUTTING_DOWN → CLOSED
```

### Listener

```
CREATED → STARTING → READY → RECEIVING
RECEIVING → PAUSED → RECEIVING
RECEIVING → STOPPING → STOPPED
STARTING → ERROR
RECEIVING → ERROR
ERROR → RESETTING → STARTING
```

### Session

```
CREATED → OPENING → OPEN → AUTHENTICATING → AUTHENTICATED
AUTHENTICATED → SUSPENDED → AUTHENTICATED
AUTHENTICATED → CLOSED
OPEN → EXPIRED
```

### Circuit

```
UNLOADED → LOADING → LOADED → STARTING → RUNNING
RUNNING → PAUSING → PAUSED
PAUSED → RESUMING → RUNNING
RUNNING → STOPPING → STOPPED
LOADING → FAILED
STARTING → FAILED
RUNNING → FAILED
```

## 7. GUI-to-client listener transition

The normative client/listener startup sequence is:

1. `GUI.CREATE`
2. `CLIENT.CREATE`
3. `LISTENER.CREATE`
4. `SESSION.CREATE`
5. `CLIENT.CONNECT`
6. `LISTENER.START`
7. `SESSION.OPEN`
8. `SESSION.AUTHENTICATE` when required
9. `CIRCUIT.LOAD`
10. `CIRCUIT.START`
11. `LISTENER.SUBSCRIBE`
12. `GUI.SHOW`
13. `APP.READY`

Runtime interaction then follows:

```
GUI action
   ↓
EVENT.EMIT
   ↓
COMMAND.REQUESTED
   ↓
COMMAND.INVOKE
   ↓
SLeeLa Client
   ↓
Circuit / Service
   ↓
LISTENER.RECEIVE
   ↓
LISTENER.ACK
   ↓
DATA.UPDATE / MONITOR.EVENT
   ↓
CONTROL.UPDATE
   ↓
GUI
```

## 8. Listener message envelope

A Guia™ listener message SHOULD contain:

```text
protocol
version
messageId
sequence
timestamp
sessionId
clientId
listenerId
event
source
target
payload
status
error
```

`sequence` provides ordered delivery detection. `messageId` provides message identity. Implementations MAY provide transport-specific framing underneath this logical envelope.

## 9. Acknowledgement

Listeners use:

```
LISTENER.ACK(messageId, sequence)
LISTENER.NACK(messageId, sequence, reason)
```

ACK confirms receipt and acceptance of the message. NACK identifies a message that could not be accepted. A transport MAY separately provide delivery guarantees; Guia™ does not assume that every transport is reliable.

## 10. Binding model

Bindings connect GUI properties to SLeeLa data:

```
CONTROL → DATA
CONTROL ← DATA
CONTROL ↔ DATA
EVENT → COMMAND
COMMAND → DATA
DATA → EVENT
MONITOR → DATA
```

A binding MUST identify source, target, direction, conversion rules, and optional validation.

## 11. Error model

Standard categories:

```
GUI_ERROR
CLIENT_ERROR
LISTENER_ERROR
SESSION_ERROR
CIRCUIT_ERROR
COMMAND_ERROR
DATA_ERROR
AUTHORIZATION_ERROR
VALIDATION_ERROR
PROTOCOL_ERROR
TIMEOUT_ERROR
TRANSPORT_ERROR
```

Errors SHOULD contain:

```
code
message
source
operation
objectId
messageId
recoverable
details
```

## 12. Security boundary

Guia™ does not grant authority merely because a GUI requests an operation. The SLeeLa runtime remains authoritative for permissions, filesystem access, networking, process execution, and administrative operations.

## 13. JavaFX mapping

JavaFX is an adapter for Guia™, not the protocol itself.

Examples:

```
CONTROL.CREATE(button)
    → JavaFX Button

EVENT.BIND(button, ACTION)
    → JavaFX event handler

DATA.SUBSCRIBE(model)
    → JavaFX observable/binding layer

CONTROL.UPDATE
    → JavaFX property update

WINDOW.CLOSE
    → JavaFX Stage close
```

## 14. Skya integration

Skya's JavaFX client SHOULD use the sequence:

```
SkyaClient.sleela
      ↓
CIRCUIT.LOAD
      ↓
CIRCUIT.START
      ↓
LISTENER.START
      ↓
MONITOR.SUBSCRIBE
      ↓
JavaFX Guia™ adapter
      ↓
Skya client monitor
```

The JavaFX monitor is therefore a Guia™ client rather than a second independent control system.

## 15. Compatibility requirements

A Guia™ implementation MUST:

- identify its protocol version;
- expose its supported capabilities;
- preserve object IDs during an active session;
- reject unknown mandatory operations safely;
- report state transitions;
- preserve message sequence information;
- respect SLeeLa authorization;
- avoid silently changing security policy.

Future revisions MUST document incompatible changes and assign an appropriate protocol version.

## 16. Reference status

This document is the normative starting reference for **Guia™** within SLeeLa.

**Guia™ 1.0 — SLeeLa GUI Protocol™**

— Max Rupplin - MEARVK LLC - 2026
