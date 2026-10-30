# Help / reach-out desk (C + C++)

A premium reach-out subsystem: a user submits a **Question or Query**, and the
program lists the **respondents** who can address it — Management, co-series
authors, telegenically-aware authors, mayors, and city spokespersons — with
each respondent's reach channels and whether they are **alive** and **known of
Science**.

## Data boundary (read this)

This library ships **no real personal contact data**. The bundled directory is
**synthetic placeholder data** on reserved, non-routable ranges:

- emails at `example.com` / `example.org` (RFC 2606),
- phone numbers in the fictitious `555-01xx` block,
- IPs in `192.0.2.0/24`, `198.51.100.0/24`, `203.0.113.0/24` (RFC 5737).

The library performs **no network I/O** — it never emails, calls, or connects
to any IP or port; it only stores and lists records. Load your own **real,
authorized** contacts later through `sl_help_desk_add_contact` /
`HelpDesk::addContact` (that is the loader seam); the placeholder generator is
only for demos and tests.

## What a contact has

`id`, `name`, `organization`, `email`, `phone`, `remote_ip` + `remote_port`,
`role`, a channel bitmask (email / phone / remote), `topic_tags`, and two status
flags: `alive` and `known_of_science`.

Roles: management, co-series author, telegenically-aware author, mayor, city
spokesperson.

## Query → respondents → status

`sl_help_desk_query` (C) and `HelpDesk::ask` (C++) match the query terms,
case-insensitively, against each contact's name, organization, role, and topic
tags, rank by number of terms matched, and return respondents carrying their
`alive` / `known_of_science` status. `*_query_role` / `askRole` filter to one
role.

The C++ wrapper is inheritable: override `reachable()` to apply policy.
`ScienceHelpDesk` demonstrates a policy that surfaces only living respondents
known of Science.

## Sizing and scaling

Built to hold about **500 contacts for a company of ~1000** (both are
constructor parameters). Query matching is a single linear scan — **O(n)** in
the number of contacts — so cost scales **linearly** with directory size. The
reported `total` reflects the whole directory even when only a small result
window is requested.

## Build, test, run

    make
    make test
    ./build/help-example-c "city transit mayor"      # query from argv
    echo "research science methods" | ./build/help-example-c
    ./build/help-example-cpp
