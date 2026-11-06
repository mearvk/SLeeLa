# Native and exec command forms

```sh
sleela native -- path/to/program.sleela
sleela exec path/to/program.sleela
sleela native --mm=512M -- path/to/program.sleela
```

The native form accepts `--config FILE`, the memory-manager option, and
`--` to end option parsing before the program and its arguments. Example
configuration path (only if present in the checkout):

```sh
sleela native --config config/vm.conf -- path/to/program.sleela
```

`exec` is documented as the corresponding execution form.
