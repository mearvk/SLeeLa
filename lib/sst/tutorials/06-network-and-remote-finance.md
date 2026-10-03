# SST Tutorial 06 — Network and Remote Financial Systems

This tutorial combines network and remote-system concepts without treating the example as a real financial protocol.

## Network layer

~~~sst
system "NetworkSystem" {
  group "network";
  name "EdgeRouter";
  component "Resolver";
  component "Transport";
}
~~~

## Remote financial layer

~~~sst
system "RemoteFinancialSystem" {
  group "finance.remote";
  name "RemoteLedger";
  component "AccountService";
  component "TransactionService";
  component "AuditService";
  remote true;
}
~~~

The two systems can be composed into a larger architecture:

~~~text
network.edge
    |
    v
PaymentRouter
    |
    v
finance.remote
    |
    v
RemoteLedger
    +-- AccountService
    +-- TransactionService
    +-- AuditService
~~~

This is an architecture model only. It does not specify authentication, authorization, settlement rules, accounting controls, or a production financial protocol.

## Exercise

Add an identity component and an audit component. Keep them separate from transaction processing so their responsibilities remain explicit.
