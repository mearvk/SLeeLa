# Law: Moral Reasoning, Contracts, and Public Standards

This directory contains a carefully labelled starter corpus for legal reasoning and structured interviews. It is designed for educational model development, LSAT-style analytical practice, contract-principle exploration, and reflective discussion of public-service standards.

## M/I: morals and intellect

**M/I** is a conceptual lens for studying how moral commitments and intellectual reasoning interact. It is not a validated psychological scale, a measure of intelligence, or a formula for ranking people. The corpus does not assign a numeric moral score or infer a person's inner character from a short answer.

The model should learn to:
- identify the principle a speaker says they are applying;
- distinguish a yes/no position from the reasons offered for it;
- identify assumptions, relevant facts, counterexamples, and uncertainty;
- notice tensions between stated principles and a described action without declaring the speaker dishonest;
- explain how consent, promise, reliance, fairness, remedy, and public trust can shape rules.

## Files

| File | Contents |
|---|---|
| `interview-standards.jsonl` | Binary opening questions followed by paragraph prompts that invite explanation |
| `contract-principles.jsonl` | Contract formation, consent, reliance, good faith, breach, and remedies |
| `lsat-reasoning.jsonl` | Argument structure, assumptions, inference, flaw detection, and conditional logic |
| `public-service-judgment.jsonl` | Ethical scenarios relevant to public service and diplomatic work |

## Record format

Each file is UTF-8 JSON Lines. Every line is one JSON object with `prompt` and `completion` strings, compatible with the minimal supervised format documented in the parent `datasets/README.md`.

Interview prompts use a two-stage structure:
1. **Binary position:** ask the respondent to answer Yes or No (or explicitly state that neither answer fits).
2. **Explanatory paragraph:** ask what the answer means, which principle supports it, what facts could change it, and what fair exception might apply.

The binary answer is a starting point, not a complete evaluation. Preserve the paragraph as the primary evidence of reasoning. Do not treat vocabulary sophistication, verbosity, accent, education, or agreement with a preferred political or moral position as proof of integrity or competence.

## Responsible use

- These are synthetic educational examples, not validated LSAT questions, legal advice, official State Department interview questions, or government selection criteria.
- Do not use this corpus to automate hiring, security-clearance, immigration, or other high-impact decisions.
- Do not infer honesty, loyalty, empathy, or moral worth from a single response. Evaluate the quality of the argument, not a presumed hidden trait.
- Keep assessment criteria job-related, transparent, consistently applied, and open to human review.
- Do not train on identifiable interviewee responses without appropriate notice, consent, data minimization, retention limits, and legal authorization.
- Keep jurisdiction-specific legal rules separate from general ethical principles; law varies by jurisdiction and changes over time.

## Suggested labels for downstream annotation

`domain`, `topic`, `response_mode`, `reasoning_skill`, `principle`, `uncertainty_present`, `counterexample_present`, and `source_type`.

Use `source_type: synthetic` for every record in this initial corpus. If future examples are based on statutes, cases, or official materials, add the jurisdiction, citation, retrieval date, and rights/attribution information rather than silently blending them with synthetic examples.
