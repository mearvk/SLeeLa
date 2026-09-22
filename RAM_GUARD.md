# RAM Guard command-line configuration

SLeeLa's command-line native runner can select a common configuration file while
passing executable arguments through unchanged.

## Run an executable with a common config

```text
sleela exec --config config/sleela.properties.example -- ./program --input 42 --ram-policy 0.95
```

The `--` ends SLeeLa options. Everything after the executable is passed to the
specified executable, including flags with numeric values.

The selected configuration path is exported to the child as:

```text
SLEELA_CONFIG_FILE
```

The executable may then parse that common properties file using its own
application-specific rules.

## RAM target profile

The example configuration expresses the requested basic one-stick reduction:

```text
ram.guard.enabled=true
ram.guard.sticks=1
ram.guard.policy=0.95
ram.guard.direction=decrease
ram.guard.write=false
```

`0.95` means a requested target of 95% of the selected stick's detected or
configured baseline frequency (a 5% reduction).

This is a target/configuration value, not a promise that arbitrary hardware can
change DRAM timing or voltage. `ram.guard.write=false` keeps the common profile
read-only/configuration-oriented until a verified hardware-specific controller
is available.

## Command-line forms

```text
sleela native --config FILE -- PROGRAM ARG1 ARG2
sleela exec   --config FILE -- PROGRAM ARG1 ARG2
sleela exec   --config=FILE -- PROGRAM --number 5 --factor 0.95
```

The existing SHA-256 execution gate remains in force before native execution.
`--memory-manager` can be combined with `--config`.
