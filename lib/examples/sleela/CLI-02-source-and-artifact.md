# Source and Core artifact lifecycle

```sh
sleela version
sleela check CLASSIC-hello-world.sleela
sleela run CLASSIC-hello-world.sleela
sleela compile CLASSIC-hello-world.sleela -o /tmp/hello.sleela
sleela validate-artifact /tmp/hello.sleela
sleela run /tmp/hello.sleela
```

`check` validates source without executing it. `compile` creates a persistent
runnable Core artifact. `validate-artifact` checks its ABI/format. `run`
executes supported source or an artifact. A passing check does not prove that
runtime behavior is correct.
