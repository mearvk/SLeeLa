# Sleela Script Configuration

Sleela Script uses the SLeeLa unified configuration system.

The authoritative repository configuration remains:

`config/sleela.conf`

The runtime configuration root is the canonical absolute root resolved by the
SLeeLa configuration layer. The scripting subsystem must not create a separate
per-user or per-language configuration tree.

Recommended runtime layout:

```
<CANONICAL_CONFIG_ROOT>/
  scripting/
    scripting.conf
    modules/
```

The source template for scripting defaults belongs in this directory:

`sleela-scripting/scripting.conf`

The active configuration is resolved as:

`<CANONICAL_CONFIG_ROOT>/scripting/scripting.conf`

## Example

```ini
[scripting]
version = "1"
enabled = true
default_extension = ".sleela-script"
max_steps = 1000000
max_recursion = 256
module_root = ""
restricted_mode = true
```

`module_root` is optional. If empty, the host uses the project configuration
root's scripting module directory.
