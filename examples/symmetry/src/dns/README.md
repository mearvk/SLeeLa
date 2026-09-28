# DNS Server Example

A SLeeLa symmetry example for a small DNS service with explicit Client, Server, and Admin/Monitor roles.

## Components

- `server/System.sleela` — authoritative service model and request lifecycle.
- `client/System.sleela` — resolver client model.
- `admin/System.sleela` — administrative and monitoring model.

This example is intentionally protocol-oriented: it models DNS query/response flow, server state, administrative observation, and controlled configuration without binding the example to a particular operating-system DNS daemon.
