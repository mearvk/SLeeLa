# Decompiler examples — what to learn here

These examples teach how to **read a compiled artifact safely**: inspect its
structure before trusting it, parse its section map, and disassemble its
bytecode into readable mnemonics. No real artifact is ever decoded — the inputs
are small in-memory stand-ins — so every run is deterministic and side-effect
free.

Read them in order.

## The three cases

| Level | File | The one new idea |
|---|---|---|
| Basic | [`BASIC-artifact-inspection.sleela`](BASIC-artifact-inspection.sleela) | A safe inspection workflow: identify → verify digest → inspect → preserve → compare. |
| Moderate | [`MODERATE-section-table.sleela`](MODERATE-section-table.sleela) | Parse a header + section table (name, offset, size) and sanity-check it. |
| Advanced | [`ADVANCED-disassemble-stream.sleela`](ADVANCED-disassemble-stream.sleela) | Walk a bytecode stream, decode each opcode, and read its operands. |

## What we want you to take away

1. **Structure before code.** Every real format — ELF, Java class files, SLeeLa
   artifacts — is a header followed by a map of sections. A decompiler parses
   that map *first* and never assumes it is well-formed: the moderate example
   checks that no section overlaps the header and that the bytes add up.
2. **Disassembly is a decode loop.** The advanced example is the heart of any
   disassembler: read one opcode, look up its mnemonic, and advance the program
   counter by 1 or 2 depending on whether the instruction carries an operand.
   Getting operand lengths right is what keeps the stream aligned.
3. **Inspect without modifying.** Step 4 of the basic workflow — *preserve the
   original artifact unchanged* — is a safety rule. You verify a digest before
   you look, and you compare your output to a known-good fixture afterward.

## Practical notes for SLeeLa 1.7

- **Object arrays** use the standard idiom: a static-field array, elements built
  in typed locals and stored back, read via typed locals (`Section s =
  sections[i];`). See the moderate example.
- **A program counter is just an int.** The advanced loop advances `pc` by the
  instruction length and stops at the `HALT` opcode by jumping `pc` to the end —
  the teaching-file way to "break".

## Run them

```sh
sleela run lib/examples/decompiler/MODERATE-section-table.sleela
sleela run lib/examples/decompiler/ADVANCED-disassemble-stream.sleela
```

Output is deterministic: the section table accounts for 800 bytes total; the
disassembler prints `PUSH 6 / PUSH 7 / MUL / PRINT / HALT` and decodes 5
instructions.
