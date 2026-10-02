# Nordshrift Tutorial 05 — Resolving Systems

Nordshrift can treat system declarations as structured source entities before emitting SLeeLa.

## Resolution model

~~~text
SST system
   |
   +-- system identity
   +-- group
   +-- name
   +-- components
   +-- remote boundary
   |
   v
Nordshrift semantic representation
   |
   v
SLeeLa
~~~

For example:

~~~sst
system "NetworkSystem" {
  group "network";
  name "EdgeRouter";
  component "Resolver";
  component "Transport";
}
~~~

The group and name remain distinct fields. This allows a resolver to organize systems by group without changing their declared names.

## Exercise

Resolve the systems in lib/nordshrift/examples/system-naming.sst and verify that finance.* and network.* remain separate groups.
