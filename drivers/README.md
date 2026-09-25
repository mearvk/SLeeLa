# Drivers

The repository-level `/drivers` directory defines the shared driver architecture and completeness contract used by SLeeLa driver families.

## Responsibilities

The driver architecture covers:

- hardware identity and model/version metadata;
- pin roles, direction, polarity, pull configuration and voltage constraints;
- memory/register regions and access permissions;
- cache and DMA-coherency policy;
- control-plane operations;
- bounded data-plane transport;
- buffering, backpressure and sequence tracking;
- producer/consumer message passing;
- mutex and condition-variable synchronization;
- CRC-32 frame integrity;
- transport, command, data, state, error and recovery paths;
- model-specific capability and evidence boundaries.

The concrete Skya telephony implementation lives under:

`telephony-skya/drivers/`

That directory is the authoritative implementation location for the Skya driver family. Repository-level `/drivers` is the shared architecture/documentation boundary and must not invent device-specific mappings.

## Completeness rule

Infrastructure being implemented does not imply that every device model is hardware-complete.

A model is hardware-complete only when its applicable device-specific paths have been audited and the repository records the evidence, hardware mappings, supported operations, unsupported operations, assumptions and test scope.

See `DRIVER.COMPLETENESS.md` and the terminology in `/TERMINOLOGY.md`.
