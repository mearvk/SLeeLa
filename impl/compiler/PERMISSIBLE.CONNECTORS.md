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

## Category

**Category:** Clean and Tech.

A Permissible Connector belongs to the Clean and Tech category when its source, configuration, execution path, and resource behavior remain explicit, bounded, auditable, and technology-oriented. The category describes the connector's operating discipline; it is not a claim that an external system is inherently safe or clean.

## Definition

**Definition:** A Permissible Connector is a bounded connector whose permitted methods and resources are identified before execution and whose operation remains within declared Seraph, Memory, Balance, Admin Safe, and Journey Safe controls.

Permissible Connectors should start clean and tech-focused, remain clean and tech-focused throughout their lifecycle, and preserve that character during resolution, construction, loading, execution, and unloading.

## Permissible Connector Memory

Permissible Connector Memory is a separate logical memory domain for Permissible Connector state and resource accounting. The design permits a Plus or Admin execution context to place connector state in this dedicated memory domain rather than mixing it with unrelated application state.

This is an isolation boundary and accounting model, not a guarantee of physical memory separation by the operating system. An implementation may map the logical domain to an actual process, allocator, arena, sandbox, VM memory region, or other enforced boundary when the platform supports it.

Software executed in Permissible Connector Memory is expected to operate normally within the declared memory limits. The connector layer must not silently require unrestricted global memory, undeclared state, or resources outside its admitted capabilities. Resource exhaustion, denied capabilities, or invalid transitions must produce controlled failure states rather than undefined behavior.

For **Plus** and **Admin** purposes, the intended sequence is:

    allocate/enter Permissible Connector Memory
    -> establish Seraph identity
    -> establish Memory limits
    -> establish Balance rules
    -> perform Admin Safe/Journey Safe checks
    -> construct/load software
    -> execute within declared limits
    -> release/exit Permissible Connector Memory

The logical memory domain remains subordinate to normal compiler, VM, operating-system, permission, and security controls.

