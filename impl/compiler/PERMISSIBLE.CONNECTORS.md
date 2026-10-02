# Permissible Connectors

Permissible Connectors are a distinct source-level connector theme for explicitly identifying and controlling bounded methods involving sight, naming, travel, processes, memory, systems, and linear graphs.

## Import

    import :: permissible :: connector :: mysql :: /lib/etc/Permissible.sleela;

A single unnamed connector is also accepted:

    import :: permissible :: connector :: /lib/etc/Permissible.sleela;

A named form is also accepted:

    import permissible connector mysql = /lib/etc/Permissible.sleela;

## Domains

    permissible domain mysql :: sight;
    permissible domain mysql :: name;
    permissible domain mysql :: travel;
    permissible domain mysql :: process;
    permissible domain mysql :: memory;
    permissible domain mysql :: system;
    permissible domain mysql :: linear-graph;

## Seraph, Memory, and Balance

Seraph represents connector identity and continuity. Memory represents bounded state and resource accounting. Balance represents constraints that keep transitions, resources, and graph movement bounded.

## Admin Safe and Journey Safe

Admin Safe and Journey Safe are explicit policy checks. They do not guarantee safety by name alone; admission requires applicable capabilities, permissions, resource limits, routes, state transitions, and audit/security checks to be verified.

## Secure Future

secureFuture represents an admitted state after policy checks. It is a policy state, not a promise about external systems or future events.

Flow: import -> identify -> domain -> configuration/properties -> capability review -> Admin Safe/Journey Safe -> Seraph/Memory/Balance validation -> deferred construction -> VM load -> bounded execution.

Permissible Connectors do not bypass normal parsing, semantic analysis, dependency resolution, capability checks, security policy, or VM controls.
