# SLeeLa National Banking Data Workflow

## Purpose

This document describes the workflow that populates and validates the national banking and economic data set for the project-defined 391-country/economy registry.

## GitHub Actions Workflow

The executable workflow is:

`/.github/workflows/populate-banks.yml`

It can be started manually with GitHub Actions and is also scheduled weekly.

### Schedule

- Manual trigger: `workflow_dispatch`
- Scheduled trigger: Mondays at 03:17 UTC

## Processing Sequence

1. Check out the repository.
2. Validate that Python 3 is available.
3. Run `tools/banking/populate_banks.py`.
4. Read the canonical registry from `data/banking/registry.json`.
5. Generate `data/banking/national_banking.json`.
6. Regenerate the national table in `BANKS.md`.
7. Require exactly 391 registry records.
8. Commit generated changes when changes exist.
9. Push the generated commit back to the repository.

## Required Inputs

The registry must contain exactly 391 canonical country/economy records before a successful population run is possible.

Current bootstrap state must be replaced with the project's actual 391-entry registry before the invariant can pass.

## Primary Files

- `BANKS.md` — schema, partitions, validation rules, and generated national table.
- `tools/banking/populate_banks.py` — population and generation program.
- `data/banking/registry.json` — canonical 391-entry input registry.
- `data/banking/national_banking.json` — generated machine-readable output.
- `.github/workflows/populate-banks.yml` — executable GitHub Actions workflow.

## Validation Invariant

The workflow requires:

```text
expected == 391
len(records) == 391
```

If either condition fails, the workflow stops rather than producing an incomplete national data set.

## Data Provenance

The population process is intended to preserve provenance and distinguish verified data from missing or partial data. Banking institutions, regulators, founding/state-formation dates, economic influence, corporate struggles, and socialist-period history should not be inferred merely to fill a record; they should be supplied from appropriate authoritative or documented sources.

## Result

A successful run produces a synchronized national data table and machine-readable data file, then commits those generated changes to the repository.
