<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-logo-004.jpg" alt="SLeeLa">

<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian-logo.png" width="75" height="75" alt="SLeeLa">

# SLeeLa HTTP 6.0

**Status:** Experimental SLeeLa application-protocol generation; not an IETF HTTP/6 standard.

HTTP 6.0 consolidates the application capabilities introduced by HTTP 5.0 into a portable **Consolidated Friends' Bet** record. The record can contain a friends list, point values, document references, a team/area label, a user-supplied consolidated IQ value, debate material, and an optional recipient label.

The record is ordinary application data carried through an authorized HTTP exchange.

## Consolidated Friends' Bet

A Friends' Bet is a user-created, non-binding application record containing:

- friends list;
- optional point values;
- assigned document references;
- team/area label;
- consolidated IQ value supplied by the user;
- debate topic;
- user-authored position or argument;
- optional recipient label.

The term **bet** identifies the application record. It is not a gambling service or financial instrument.

## Teamster Debate

The Teamster Debate option provides a structured communication package:

1. Identify the team area.
2. Supply the user's consolidated IQ value as application data.
3. Supply a debate topic.
4. Supply a user-authored position or argument.
5. Identify the intended recipient or audience.
6. Serialize the package into an HTTP 6.0 application packet.

Recipients may accept, reject, ignore, or filter the resulting communication.

## HTTP 5.0 → HTTP 6.0

HTTP 6.0 carries forward:

- Friends' Packs;
- friends lists;
- points;
- document references;
- bonus-offer references;
- FP accounting;
- frame/session concepts;
- audit concepts.

It adds:

- `CONSOLIDATED_FRIENDS_BET`
- `TEAMSTER_DEBATE`
- `CONSOLIDATE_IQ`
- `TEAM_AREA`
- `DEBATE_TOPIC`
- `DEBATE_POSITION`
- `RECIPIENT_LABEL`

## Initial Pack Representation

The initial representation is deliberately inspectable:

```text
friend-list + points + documents + team-area + IQ + debate
```

The binary frame layer carries a bounded serialized application payload.

## Safety and Authorization

HTTP 6.0 packets are application data. They do not authorize interference with networks, systems, persons, property, or communications.

Application data should be transmitted only through authorized channels and subject to the receiving system's policies.

## Build

```sh
make -C http-6.0
```