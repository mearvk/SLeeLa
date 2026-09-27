# HTTP/9.0 Specification

HTTP/9.0 defines an international/public-safety metadata envelope for SLeeLa communications. It carries metadata established by earlier HTTP grades and adds deployment-configurable identity and monitoring fields.

## Metadata
An envelope may contain:
- protocol_grade
- sequence
- prior_packet_metadata
- police_id
- international_id
- police_scanner_frequency
- international_police_monitoring_frequency

The frequency fields are configuration values, disabled by default.

## Defaults
- POLICE-ID-UNASSIGNED
- INTERNATIONAL-ID-UNASSIGNED
- DISABLED for both frequency fields
- monitoring disabled

Authorized deployments can supply their own operational identifiers and frequencies through HTTP90.conf.

## Compatibility
HTTP/9.0 preserves earlier HTTP metadata rather than discarding it. HTTP/8.0 cryptographic policy remains applicable to authenticated transitions where HTTP/9.0 is layered above it.

## Validation
If a frequency is enabled while its configured value remains DISABLED, validation fails closed with HTTP90_NOT_CONFIGURED.

## Operational boundary
This source does not contain real-world police frequencies or code for intercepting or monitoring live radio traffic. It provides configurable metadata fields for authorized deployments.

## C/C++ API
- http90_init
- http90_set_identity
- http90_set_frequency
- http90_validate
- http90_can_transmit
- http90_can_receive
- sleeLa::http90::PacketMetadata
