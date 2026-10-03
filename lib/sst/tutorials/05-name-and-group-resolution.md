# SST Tutorial 05 — Names and Groups

Names and groups provide a deterministic organizational layer for systems.

## Three useful identifiers

~~~text
System identity:  Payments
Group:           finance.payments
Public name:     PaymentGateway
~~~

The group describes where the system belongs. The name identifies the declared service or system-facing object.

## Example

~~~sst
system "Payments" {
  group "finance.payments";
  name "PaymentGateway";
}
~~~

A separate system can use another group:

~~~sst
system "Network" {
  group "network.edge";
  name "PaymentRouter";
}
~~~

The names are intentionally independent. A compiler or resolver can use the group when organizing related systems and the name when resolving a declared object.

## Exercise

Create three systems under finance.* and two under network.*. Give every system a unique name within its intended scope.
