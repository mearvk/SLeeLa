# HTTP 4.0 Profiles

Profiles are named bundles of negotiated behavior. They are SLeeLa configuration concepts, not HTTP standards.

## baseline

- FRAME_BASE
- ordered frame processing
- bounded payloads

## incremental

- baseline
- INCREMENTAL
- DATA forwarding before logical response completion

## resumable

- baseline
- RESUMABLE
- RESUME continuation
- persistent transfer identifier

## transport

- baseline
- INCREMENTAL
- RESUMABLE
- FLOW_CONTROL
- CAPSULE

## migration

- transport
- MIGRATION
- authenticated session state across carrier/path changes

Profiles must only enable capabilities present in the negotiated intersection.
