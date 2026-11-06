# SleelaDoc facade search and resource bounds

```sh
sleela docs --facade text
sleela docs --facade text --facade-root ./lib
sleela docs --facade text --facade-root ./lib --facade-max-mb 12 --facade-max-files 12000 --facade-max-seconds 120
```

The facade mode searches filesystem folders and files and renders discovered
SLeeLa source pages. Documented defaults are 6 MB, 6000 files, and 360 seconds.
The command reports when a cap stops traversal. Use a narrow root and sensible
limits; do not point it at an unbounded filesystem casually.
