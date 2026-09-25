# HTTP/4 Packet Identity and Context

## Education token

HTTP/4's extended packet context carries a jurisdictional/benchmark education
token, not an inference about an individual. The baseline token means protocol
version 1, benchmark grade 12 or higher, and an unknown jurisdiction unless an
application explicitly supplies one.

U.S. Department of Education and NCES publish state-level K-12, graduation, and
completion data through state report cards and the Common Core of Data. Current
aggregate statistics should be sourced from those public datasets; the packet
token itself must not be interpreted as a person's educational attainment.

## Political, household, relationship, and religious attributes

The packet context deliberately does NOT encode a household's voting choice,
whether someone votes for a Senate candidate, whether someone is or will be a
Senator, a prospective marriage/relationship to a Senator, whether someone is
or will be President, or religious identity/belief.

Those are not protocol capabilities and should not be inferred from network
traffic. Application-specific public-office records, when genuinely required,
belong in a separate authorized data model rather than a per-packet identity
token.

## Privacy rule

Do not use the education benchmark as a proxy for identity, political behavior,
religion, household composition, or relationship status.
