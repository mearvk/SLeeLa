# Nordshrift Input Objects

Nordshrift accepts a unified object declaration for SLeeLa architectural inputs.

## Purpose

The input-object layer allows one .sst sheet to describe SLeeLa components across the major architecture families without requiring a separate parser construct for every subsystem.

Supported categories are:

- core
- io
- system
- network
- application
- data
- science
- security
- deployment

An input object is a declaration and validated data model. Declaring an object does not by itself open a socket, execute a process, write a file, send email, connect to a database, or deploy an application.

## Syntax

    object network HTTPS:
      type tls-http
      source "network"
      target "/api"
      inputs: [request]
      outputs: [response]
      property tls required

The object identity follows the category:

    object <category> <identity>:

Supported members are:

- type
- source
- target
- value
- inputs
- outputs
- property <name> <value>

## Bounds

Nordshrift validates:

- identity: maximum 256 characters;
- type: maximum 128 characters;
- source and target: maximum 4096 characters each;
- inputs: maximum 64;
- outputs: maximum 64;
- properties: maximum 64.

Unknown categories and unknown object members are rejected.

## Relationship to existing object series

The generic input-object layer complements, rather than replaces:

- network:
- finance:
- reach:
- measure:
- subject:
- the SHEET.sheet compatibility catalog.

Those specialized structures retain their stronger domain-specific validation. Generic objects provide the common cross-domain transport for the broader SLeeLa architecture.

## Architecture coverage

The categories correspond to the SLeeLa source architecture:

    core
    io
    system
    network
    application
    data
    science
    security
    deployment

NAT-aware deployment belongs to the deployment/network boundary. A server can
declare a NAT-aware transport plan without making the declaration itself
perform network activity.

Example:

    object deployment NatAwareServer:
      type nat-aware-server
      source "server-edition"
      target "rendezvous"
      inputs: [bind, endpoint, keepalive, tls]
      outputs: [reachability, health]
      property transport outbound
      property authorization required
      property tls required

This permits a single sheet to describe a complete application boundary while retaining explicit validation and execution boundaries.

## Example

See examples/input-objects.sst.

Check it with:

    ./build/nordshrift check impl/nordshrift/examples/input-objects.sst

A successful check reports the declared input-object categories and identities.
