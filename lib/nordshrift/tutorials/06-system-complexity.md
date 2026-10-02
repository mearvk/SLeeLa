# Nordshrift Tutorial 06 — From Basic to Distributed Systems

Nordshrift can progressively transform increasingly complex SST system declarations.

## Basic

One system and one component.

## Network

A system with resolver and transport components.

## Remote finance

A remote system with account, transaction, and audit components.

## Distributed

A composed system that combines network, identity, financial, and auditing responsibilities.

The important compiler property is that complexity is represented structurally rather than by a single opaque name.

~~~text
system
  -> group
  -> name
  -> components
  -> boundaries
  -> semantic representation
  -> SLeeLa
~~~

## Exercise

Begin with system-basic.sst, then add the network and financial components from the larger examples. Compare the resulting semantic structure.
