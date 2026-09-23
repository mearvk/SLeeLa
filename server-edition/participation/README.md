# SLeeLa Server Participation

Each SLeeLa Server Edition may participate in a fast regroup round one or a few times per day.

## Behavior

1. Read the explicitly configured known SLeeLa servers from known-servers.conf.
2. Send a small SLEELA-MESH/1 HELLO message to all configured peers concurrently.
3. Wait for each configured peer's bounded response window.
4. Record response/no-response status without retaining peer IP addresses in the participation record.
5. Regroup after the round and continue normal server operation.
6. Repeat according to SLEELA_MESH_INTERVAL_SECONDS; the default is every 8 hours (three rounds per day).

The participation layer is bounded and explicit. It does not discover or scan arbitrary Internet addresses. A peer must be placed in the known-server configuration before it is contacted.

## Transport

The default participation port is TCP 22221. This is separate from:
- 2222 — basic probe
- 22220 — basic scan/service discovery
- 19866 — SLeeLa native server port
- 20000 — external International Strernary reference

The participation layer does not change the existing SLeeLa logical HTTP port namespace.

## Message and regroup

The initial message contains only protocol/version, configured node identifier, UTC time, and participation port. Responses are bounded to the configured timeout.

This is a coordination mechanism, not an authority mechanism. A participation response does not by itself establish legal, governmental, diplomatic, financial, identity, or safety authority.

## Configuration

Environment variables:
- SLEELA_MESH_PEERS_FILE
- SLEELA_MESH_PORT (default 22221)
- SLEELA_MESH_INTERVAL_SECONDS (default 28800)
- SLEELA_MESH_TIMEOUT_SECONDS (default 5)
- SLEELA_MESH_MAX_PEERS (default 64)
- SLEELA_SERVER_NODE_ID

round performs one immediate round. start runs repeated rounds. stop stops the participation daemon.
