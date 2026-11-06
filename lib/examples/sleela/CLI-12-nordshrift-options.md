# Nordshrift command options

```sh
sleela nordshrift --emit path/to/Program.sleela
sleela nordshrift --roundtrip path/to/Program.sleela
sleela nordshrift --target=sleela path/to/Program.sleela
sleela nordshrift --target=java path/to/Program.sleela
sleela nordshrift --target=c path/to/Program.sleela
```

The target selector chooses the documented Sleela, Java, or C target. The
default target is Sleela. Round-trip and emit modes should be tested against
the actual installed toolchain and source.
