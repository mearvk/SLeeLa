# SST Tutorial 04 — Systems of Varying Complexity

This tutorial introduces systems as named, grouped declarations.

## Level 1 — Basic system

~~~sst
system "StorageSystem" {
  group "storage";
  name "LocalStorage";
  component "FileStore";
}
~~~

## Level 2 — Network system

~~~sst
system "NetworkSystem" {
  group "network";
  name "EdgeRouter";
  component "Resolver";
  component "Transport";
}
~~~

## Level 3 — Remote financial system

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

The remote marker describes a system boundary; it does not by itself define a transport or financial protocol.

## Level 4 — Distributed system

A larger declaration can combine identity, network services, financial services, auditing, and remote boundaries.

## Exercise

Start with the basic system and add components one at a time. Keep the system identity and group stable while increasing implementation complexity.
