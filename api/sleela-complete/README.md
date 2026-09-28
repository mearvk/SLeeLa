# Sleela-Complete Module Library

This library establishes the reusable **Sleela-Complete** model for SLeeLa modules.

## Rule

A module is Sleela-Complete when its designer-facing specification can be authored in Sleela source, including its project, module, classes, fields, methods, interfaces, dependencies, capabilities, platforms, backends, adapters, connectors, security, build, tests, services, data models, workflows, endpoints, events, and examples.

Native C/C++, operating-system APIs, device drivers, and external services remain implementation substrates. They do not replace the Sleela-facing specification.

## Coverage

The class library covers the common specification layer and reusable domain layers for:
- Core/runtime
- I/O
- Network
- Security
- Data/database
- HTTP/server/router/listener
- UI/widgets
- Email/services
- AI/model/inference
- XML
- Subjects/knowledge
- Telephony/SIP/RTP/media
- Platform/backend/adapter/connector
- Build/test/documentation and module design

## Design workflow

Project -> Module -> Class -> Interface -> Field/Method -> Dependency/Capability -> Platform/Backend/Adapter -> Security -> Build -> Test -> Example -> Implementation.

Existing project-specific APIs remain authoritative for their detailed behavior. These classes provide the common designer specification surface so a new module can be designed from scratch in Sleela before native implementation is selected.

## Status

This is the Sleela-facing specification/class layer. Production behavior still requires the corresponding native implementation, platform adapter, validation, and tests.
