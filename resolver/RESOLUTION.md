# Resolver Resolution Model

SLeeLa uses one shared resolver contract rather than embedding platform-specific
DNS logic in each HTTP version.

1. If input is an IP address, reverse resolution is attempted.
2. Otherwise hostname resolution is attempted for IPv4/IPv6.
3. If DNS resolution fails, the input is treated as a filesystem path.
4. Event policy is fail-closed only when a target was explicitly configured.
5. With no target configured, the resolver hook is permissive and preserves
   existing HTTP behavior.

The resolver reports whether the result was dynamically obtained. This lets a
specific event require a newly resolved address without silently treating a
cached/static path as equivalent.

The resolver is advisory: it never installs routes, changes firewall rules, or
forces forwarding.
