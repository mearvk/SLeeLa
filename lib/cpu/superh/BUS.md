# SuperH Bus

The CPU connects to the system through a profile-specific bus interface.

Transactions contain:

- address;
- read/write;
- transfer size;
- data;
- byte enables;
- memory attributes;
- privilege;
- response/error;
- master identity.

SH-4 implementations may include a Bus State Controller and external memory interface. Renesas documents the BSC as part of SH-4 system implementations. citeturn1search8
