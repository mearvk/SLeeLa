# Global memory-manager options

Global options precede the command.

```sh
sleela --memory-manager run CLASSIC-hello-world.sleela
sleela --mm run CLASSIC-hello-world.sleela
sleela --mm=512M run CLASSIC-hello-world.sleela
sleela --memory-manager=2G check CLASSIC-hello-world.sleela
```

The bare switch enables the documented default 2 GiB limit. A supplied size
accepts byte counts with K, M, or G suffixes. Keep the option before
`run`/`check`; do not assume arbitrary option placement.
