# Sleela Script Object State

Turing 5 scripts can inspect SLeeLa objects through a typed, capability-controlled state interface.

## Read

    let state = object.read("CompilerManager")
    let version = state.version
    let status = state.status

A read normally returns a snapshot unless a host explicitly exposes a live view.

## Known variable names

The host publishes a registry of known variable and property names. Scripts request only registered names:

    let heap = object.get("VM11", "heap.used")
    let sequence = object.get("VM11", "master.sequence.index")

Unknown names are errors rather than guesses.

## Write

Writes are explicit:

    object.set("VM11", "scheduler.pauseRequested", true)

The host verifies writability, type, range and capability before applying the change.

## Transactions

Hosts may expose a transactional form:

    transaction {
        object.set(...)
        object.set(...)
        commit()
    }

Validation failure should roll back when the underlying object supports transactions.
