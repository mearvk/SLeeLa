# Multi-source compilation

```sh
sleela check path/to/Model.sleela
sleela run path/to/Model.sleela path/to/Main.sleela
sleela compile path/to/Model.sleela path/to/Main.sleela -o /tmp/application.sleela
sleela validate-artifact /tmp/application.sleela
```

Supply all source units before `-o`. Use distinct definitions; duplicate
definitions are diagnosed by the compiler.
