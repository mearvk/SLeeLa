# Advantage™ Library — Validation Record

**Status:** VALIDATED  
**Validation date:** 2026-11-08  
**Validated branch:** `master`  
**Validated source commit:** `750bc6cddbc12082f8602b953f5fad08d1466efb`  
**Validation workflow:** [Advantage Library Verification](../../.github/workflows/advantage-verification.yml)  
**CI run:** [42484773721 — successful](https://github.com/mearvk/SLeeLa/actions/runs/42484773721)

## Validation statement

The Advantage™ Library at the source commit identified above passed the repository's dedicated Advantage verification workflow. The workflow built the SLeeLa runtime from the checked-out source, checked the module's documentary artifacts, generated and inspected the Advantage API documentation, and executed all six designated demonstrations.

This record applies to the exact commit and verification environment stated here. It is not a blanket certification of every platform, deployment, financial outcome, legal interpretation, or scientific claim described by the module.

## Verification results

| Check | Result | Evidence |
|---|---|---|
| Source-built SLeeLa runtime | PASS | Runtime build completed and executable version check succeeded |
| Required documentary artifacts | PASS | Five required documents contained the `artifact;` marker |
| Preonem references | PASS | Required documents referenced `PREONEM.md` |
| Examples index | PASS | Module README referenced `EXAMPLES.md`; all six demo sources were present and documented |
| Generated API documentation | PASS | Generator completed and the Advantage package page contained all six required API class names |
| Runnable demonstrations | PASS | All six executions emitted the required `ALL CHECKS PASSED` sentinel |
| Verification report and logs | PASS | CI report and demonstration logs were uploaded as a workflow artifact |

## Demonstrations executed

All commands were run by the workflow using the source-built runtime:

1. `lib/advantage/advantage-demo.sleela`
2. `lib/advantage/math/trade-math-demo.sleela`
3. `lib/advantage/physics/physics-demo.sleela`
4. `lib/advantage/integration/integration-demo.sleela`
5. `lib/advantage/grading/grading-demo.sleela`
6. `lib/advantage/output/output-demo.sleela`

Each demonstration completed successfully and emitted the workflow's required success sentinel.

## Scope and limitations

- The recorded run used GitHub Actions on `ubuntu-latest`.
- This run validates the Linux CI path only; it does not establish Windows or macOS execution results.
- Documentation checks establish the presence of required markers, references, and documented demo paths. They do not independently certify every statement in the prose.
- API documentation checks establish successful generation and the presence of the expected Advantage API names. They are not a complete API semantic or compatibility audit.
- Demonstration results validate the included fixtures and checks, not real-world investment performance, legal compliance, or empirical physical theories. In particular, the trust-field physics material is a mathematical modelling analogy as described in the module documentation.

## Reproduction

From the repository root, the core checks performed by the workflow can be reproduced with:

```sh
cd impl
make build/sleela
cd ..
impl/build/sleela version
python3 tools/generate-sleeladoc-api.py --out /tmp/advantage-api
```

Then execute each of the six demonstration source files listed above with:

```sh
./impl/build/sleela run path/to/demo.sleela
```

The dedicated GitHub Actions workflow performs the documentary checks and requires every demonstration log to contain `ALL CHECKS PASSED`.

## CI evidence

- [Workflow definition](../../.github/workflows/advantage-verification.yml)
- [Successful run and job results](https://github.com/mearvk/SLeeLa/actions/runs/42484773721)
- Uploaded artifact: `advantage-verification-42484773721` (verification report and logs; GitHub retention applies)

---

**Validation recorded for:** SLeeLa / `lib/advantage`  
**Validated source revision:** `750bc6cddbc12082f8602b953f5fad08d1466efb`
