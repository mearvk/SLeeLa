# PDP-12 Specification

| Property | SLeeLa profile |
|---|---|
| System family | DEC PDP-12 |
| Base word width | 12 bits |
| Instruction environments | PDP-8-family and LINC-compatible modes |
| Memory | Configurable by machine profile |
| State | Shared machine memory plus mode-specific processor state |
| I/O | Configurable device map; preserve environment-specific I/O semantics |
| Cache | No modern cache hierarchy assumed |
| DMA | Device-specific behavior only when explicitly configured |
| Timing | Unspecified unless supported by a machine-specific timing table |

Evidence labels: **documented**, **derived**, **estimated**, **unspecified**. Do not assume all PDP-8 options or every LINC hardware feature exists in every PDP-12 configuration.