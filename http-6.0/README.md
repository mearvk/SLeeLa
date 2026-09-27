# SLeeLa HTTP 6.0 — Consolidated Friends' Bet

Status: Experimental SLeeLa protocol generation; not an IETF HTTP/6 standard.

## Purpose

HTTP 6.0 consolidates the application-level capabilities introduced by HTTP 5.0 into a portable Friends' Bet Pack. A pack can carry a user's friends list, point values, assigned document references, and a structured Teamster Debate option.

The pack is ordinary application data carried through an authorized HTTP exchange. HTTP itself is a request/response protocol for exchanging messages and representations; this project extension does not change that underlying standard.

## Consolidated Friends' Bet

A Friends' Bet is a user-created, non-binding application record containing:

- a simple friends list;
- optional point values;
- assigned document references;
- a team/area label;
- a consolidated IQ value supplied by the user;
- a debate topic and position statement;
- an optional recipient label.

The word bet describes the application's record. It is not a gambling service or financial instrument.

## Teamster Debate

The Teamster Debate option provides a structured package for discussion:

1. Identify the team area.
2. Supply the user's consolidated IQ value as an application value.
3. Supply a debate topic.
4. Supply a position or argument as user-authored content.
5. Identify an intended recipient or audience.
6. Pack the information into an HTTP 6.0 application packet.

The packet is designed for voluntary communication with neighbors, organizations, people in other countries, or public recipients. It does not imply endorsement, official status, authority, or receipt by any recipient.

## HTTP 5.0 Consolidation

HTTP 6.0 carries forward Friends' Packs, friend names, points, document references, Friends List file I/O, bonus-offer references, FP accounting, frame/session concepts, and audit concepts.

It adds:

- CONSOLIDATED_FRIENDS_BET
- TEAMSTER_DEBATE
- CONSOLIDATE_IQ
- TEAM_AREA
- DEBATE_TOPIC
- DEBATE_POSITION
- RECIPIENT_LABEL

## Pack format

The initial pack is deliberately inspectable:

    friend-list + points + documents + team-area + IQ + debate

The binary frame layer carries a bounded serialized application payload.

## Safety and authorization

HTTP 6.0 packets are ordinary application data. They do not authorize interference with networks, systems, persons, property, or communications.

Recipients may accept, reject, ignore, or filter packets.

## Build

    make -C http-6.0
