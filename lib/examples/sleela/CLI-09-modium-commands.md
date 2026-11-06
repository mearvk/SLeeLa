# Modium integrity commands

```sh
sleela modium status
sleela modium scan native
sleela modium scan 1
sleela modium scan native --exact
```

The manual documents `scan`, `status`, and `restore`, plus `--exact` and
`--seal` on scan. Sealing and restore can change integrity state; inspect
status and the implementation policy before using them.

```sh
# Deliberately shown as a review-before-use operation:
sleela modium scan native --exact --seal
sleela modium restore 1
```
