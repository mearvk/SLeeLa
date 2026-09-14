# SLVM `bin/`

The `bin/` directory contains command-line entry points intended for use from the SLVM environment.

## OSsupport

[`OSsupport`](OSsupport) is the command-layer entry point for native operating-system support and Defender provisioning.

```sh
OSsupport
OSsupport detect
OSsupport status
OSsupport fetch [directory]
OSsupport build [directory]
OSsupport install [directory]
OSsupport provision [directory]
```

Without arguments, `OSsupport` displays the available choices and prompts for an operation. With an argument, it is directly scriptable and preserves the exit status returned by the underlying `sleela defender` command.

The implementation uses the fixed native mapping documented in [`../OS_SUPPORT.md`](../OS_SUPPORT.md): Windows uses `mearvk/Windows.Admin.Defender`, and Linux uses `mearvk/Linux.Admin.Defender`.

After checkout, if executable permissions were not preserved by the checkout method, run:

```sh
chmod +x bin/OSsupport
```

The command does not bypass Secure Boot, UAC, Defender, driver signing, or other operating-system security controls.
