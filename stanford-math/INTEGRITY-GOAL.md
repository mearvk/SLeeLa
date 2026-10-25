# The Integrity Goal (stanford-math extended metadata)

A go-forward convention for records in this archive, building on the Metadata
Class metapattern (`SLEELA.syntax` §15) without changing the grammar — it uses
only ordinary catalogued fields + accessors, exactly as §15 sanctions.

## The extended block

Every record class carries, in addition to the baseline eleven identity
attributes, a **scientific-record block**:

```sleela
  < ---- Scientific record (purpose / method / gains / bedrock / integrity) ---- >
  String metaPurpose           = "...";
  String metaScientificMethod  = "...";
  String metaAuthorialGains    = "...";
  String metaBedrock           = "...";
  String metaIntegrityGoal     = "...";
```

…each with a `get`/`set` pair.

## The rules that make it honest

1. **Written per class, not copied.** Each field is true to *that* record's own
   standing. `SMEquation` and `SMArchive` do not share a purpose; a
   public-domain theorem and a cited 1973 paper do not share their gains.
   *Not the same.*

2. **Allow greatness to be recorded — forward, and fully.** The Integrity Goal
   exists so great moves in the universe (theorems, laws, careers, reaches) are
   recorded *in full* and carried forward, not trimmed for modesty. Record the
   great reach all the way out.

3. **…without inflating it.** The same goal forbids the opposite failure:
   crediting the archive for work it only catalogued, inventing an author or
   schedule, or claiming a brand's reach past where the record supports it.
   Where a reach stops, say so (e.g. thermodynamics coursework lives in
   Physics, not the math department).

4. **Credit flows to the real origin.** Authorial gains for a record are
   curatorial only. The science belongs to its discoverers; cited works to
   their authors and publishers. This is the §15.5 fidelity rule applied to the
   question of gains.

## Why it is a separate file

This is a deliberate, standing convention for the archive, kept greppable and
reviewable. It extends — it does not replace — the always-on
`.kiro/steering/metadata-class-standard.md`.
