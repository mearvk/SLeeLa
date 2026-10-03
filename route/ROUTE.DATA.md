# SLeeLa Route Data

This is the single authoritative route-data contract used by the Server Edition,
HTTP 1.0–9.0 implementations, and standalone server surfaces.

Required route fields:
protocol, server_surface, http_generation, vm_generation, vm_name,
config_root, route_id, target, resolution_mode, capability, transport, port,
status.

VM route identity:
- 0 /impl Core
- 1 /1 Foundation
- 2 /2 Operator
- 3 /3 Specialist
- 4 /4 Supervisor
- 5 /5 Manager
- 6 /6 Director
- 7 /7 Administrator
- 8 /8 Executive
- 9 /9 Authority
- 10 /10 Principal
- 11 /11 Sovereign

Routing rules:
1. A route identifies its protocol and server surface.
2. A route identifies the VM generation responsible for the route.
3. The canonical configuration root is resolved through config/sleela.conf.
4. Dynamic targets use the resolver before an address is accepted.
5. VM generation/name metadata never implies capability or authority.
6. HTTP generations 1.0–9.0 consume the same route contract.
7. Standalone servers use the same contract rather than a second route schema.

Consumers include server-edition, http-servers/1–3, http-1.0 through http-9.0,
api/server, api/webserver, impl/fundamental/Router, impl/extensibility/Route*,
and lib/http.
