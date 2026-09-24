# Skya™ Groups, Video Targets and Directory Search

## Video target model

The JavaFX Video surface now accepts four target classes:

- **IP** — direct network address.
- **Host** — hostname or DNS name.
- **Group** — a hosted or discovered Skya group.
- **Room** — a Skya room from the room model.

The current JavaFX layer records the requested target and presents the target boundary. Native media transport, codec negotiation, NAT traversal, encryption, and production group media routing remain separate implementation layers.

## Private groups

The normal Skya client provides a **Groups** menu:

- **Host Private Group…**
- **Search Users, Groups, Hosts & Rooms…**
- **Close Group Windows**

A private group contains a group name, owner, host/address and room. Each group receives its own JavaFX window.

The group window provides connection-oriented controls:

- Add Connection
- Remove Selected
- Video
- Audio
- Close Group

The current implementation models group membership and control state locally. It does not claim that adding a connection establishes a production network/media session.

### Group limit

The current client limits hosted private groups to **100** simultaneously managed groups.

The limit is an application-level guard and can be raised later when persistence, server-side authorization and resource accounting are implemented.

## Directory search

The client provides a separate search window for:

- Users
- Groups
- Hosts
- Rooms
- All

The search currently operates over the local client directory/model:

- the local user identity;
- locally hosted private groups;
- the current host/port endpoint;
- the built-in Skya room list.

It is therefore a discovery/search UI foundation, not yet a global network directory service. A future SLeeLa service can replace or augment these local records with authenticated remote directory results.

## Security and authority

A private-group window is a GUI control surface. It does not itself grant network authority. SLeeLa remains authoritative for authentication, authorization, connection policy, filesystem access, networking, process execution, and administrative operations.

Group membership, invitations and media permissions should eventually be validated by the SLeeLa service/circuit before a remote peer is accepted.

## Guia™

Group controls and video targeting are intended to map into the existing Guia™ GUI-to-SLeeLa protocol boundary. Future native implementations can represent group lifecycle and media operations with Guia™ commands/events without making JavaFX itself the protocol.
