# Device Instructor Manual: User Concerns

For anyone using the Device Instructor series. It is organized by the concern a
user is likely to have. Each entry says what the Instructor does and what you
can do.

## 1. How do I know the Instructor is ready?

Run lesson 3 (`DiSystemReadiness`). The Instructor is ready only when every
check reads `ready` and the verdict is "the instructor is ready to receive
input". Any other verdict lists the checks to fix.

| status | meaning | what to do |
|--------|---------|------------|
| ready | reading present, plausible, at or above the minimum | nothing |
| NOT OBSERVED | no reading was given (`-1`) | supply the reading; it is never assumed |
| IMPLAUSIBLE reading | a negative value other than `-1` | re-measure; the value cannot be true |
| BELOW MINIMUM | reading is lower than required | free resources or use a larger system |

## 2. Why was my input refused?

The Instructor always states the reason.

| message | cause | fix |
|---------|-------|-----|
| declare the field of observation first | no field was declared | declare the field, then register |
| noun already has a carrier (one place only) | the noun is registered under another carrier | keep one carrier; correct the earlier entry if it was wrong |
| registry is full | the lesson registry holds 16 entries | start a new registry |
| say what you claim | the claim was empty | write the claim in words |
| evidence is not an entry in the registry | the evidence names something unregistered | register it first, or cite the right entry |
| this step does not follow the last accepted step | steps must go 1, 2, 3 in order | submit the next step number |

## 3. The Instructor says "not observed". Is something broken?

No. It means the thing is outside the declared field, or no reading was
given. It does not mean the thing is absent in the world. Widen the field or
supply the reading.

## 4. My result and the Purchase Master's result differ.

Run lesson 4. Each difference names one noun and says whether it is a
mismatch ("re-observe") or seen by only one side. Look again at that entry
only, correct it, and compare again. The work is accepted when the verdict
is AGREE. Do not change entries to match without looking.

## 5. Is my information kept or shared?

The lessons hold everything in memory for the run and write nothing to disk
or the network. Closing the program discards it. Use only invented or
non-sensitive names in lessons.

## 6. Running problems

| symptom | cause | fix |
|---------|-------|-----|
| "SHA-256 verification required" or "mismatch" | the verified-build manifest does not match the files | regenerate it: `python3 tools/generate-sha256-manifest.py --root . --output security/sha256-manifest.json` |
| "verification tool not found" | the toolchain root is not found | set `SLEELA_HOME` to the repository root |
| "no 'main' method found" | a library class was checked alone | run a lesson that has a `main()` |
| "unexpected token '<'" after editing | a `<` comment shares a line with code | put the comment on its own line |
| "unexpected character" in a comment | an apostrophe in a comment | reword without it |

## 7. Editing the lessons safely

- Multi-line comments use `<< ... >>`. A `<` comment is one line only and
  must start the line.
- Avoid reserved words as names (for example `record`, `close`, `enum`,
  `native`).
- Keep the Metadata block. Use `none` or `unknown` when a value is not
  known; never invent an author or standard.
- After changing files under `lib/`, regenerate the manifest and library
  inventory as described in the repository steering notes.

## 8. Scope and honesty

The Instructor checks form: a declared field, registered evidence, order,
plausible readings and agreement. It does not judge whether a student is
right about the real world beyond the evidence in the field.
