# Nordshrift Tutorial 07 — SQL Connector Resolution

Nordshrift resolves SQL connector declarations into a provider-specific semantic boundary.

SST -> provider connector -> Nordshrift semantic connector -> native provider boundary

The native boundary currently validates configuration and reports that provider client loading is not yet implemented.

## Installation boundary

Future installation belongs below semantic resolution: resolve provider, locate/install client, load provider library, then connect.

This separation allows installation policy to be added later without changing the SST source model.
