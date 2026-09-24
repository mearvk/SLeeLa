# Guia™ State Transitions Reference

## GUI
`CREATED → INITIALIZING → READY → ACTIVE → PAUSED → ACTIVE → CLOSING → CLOSED`

## Client
`CREATED → CONNECTING → CONNECTED → READY`; reconnect uses `DISCONNECTED → RECONNECTING → CONNECTED`.

## Listener
`CREATED → STARTING → READY → RECEIVING → STOPPING → STOPPED`; failures use `ERROR → RESETTING → STARTING`.

## Session
`CREATED → OPENING → OPEN → AUTHENTICATING → AUTHENTICATED`; sessions may suspend/resume or close/expire.

## Circuit
`UNLOADED → LOADING → LOADED → STARTING → RUNNING`; runtime supports pause/resume and stop; load/start/run failures enter `FAILED`.

Transitions MUST be observable through Guia™ events and MUST NOT silently skip required authorization or lifecycle validation.
