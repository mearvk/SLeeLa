artifact;
pure;
gasoline;

# lib/advantage/examples/professionals — examples cast for professional roles

Worked `advantage` examples framed around professionals: grading professional
roles as modules and ranking them A–D with the sophistication circuit grader.

| Example | What it shows |
|---------|---------------|
| `professionals-demo.sleela` | Scores a set of professional roles and prints each one's rating plus the winning set, using the `grading/` subject. |

Run it:

```sh
./working-binaries/sleela run lib/advantage/examples/professionals/professionals-demo.sleela
```

Naming the grading classes pulls them in from `lib/advantage/grading/` via the
driver's `/lib` identifier scan; no imports are needed.
