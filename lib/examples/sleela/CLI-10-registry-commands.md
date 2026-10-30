# Registry commands

Start with read-only inspection:

```sh
sleela registry
sleela registry review
sleela registry get KEY
sleela registry feature NAME
```

The command family also documents `set`, `save`, and `export`:

```sh
sleela registry set KEY VALUE
sleela registry save
sleela registry export
```

Replace placeholders with keys/features that exist in the registry. Treat
`set` and `save` as mutations and inspect the registry guide first.
