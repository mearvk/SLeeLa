# SLeeLa Philosophy: Conversation Mask and Integer Reduction

**Status:** Initial, non-clinical reference implementation  
**Language:** C11 API and an inheritable C++17 wrapper

## 1. Purpose

This module provides a deterministic foundation for a polymorphic Conversation Mask and Integer Reduction.

It answers a narrow engineering question: given observations that a person or research source has explicitly supplied, which observations may be shown in a conversation, what is their provenance, and how can one declared numeric observation be reduced to a bounded integer without changing its meaning?

It does **not** calculate a person's intelligence, wisdom, maturity, moral worth, employability, parenting ability, or suitability for a relationship or profession. There is no aggregate person score. The program does not diagnose, predict, or recommend treatment.

## 2. Terminology

| Term | Meaning in this module |
|---|---|
| Integer Reduction (IR) | Convert one finite, declared numeric value into an integer using a documented rounding rule and explicit lower/upper bounds. It is numeric quantization, not reduction of a human being. |
| Conversation Mask | A display policy that filters observations according to their consent-to-share flag, requested subject domains, and—in C++—application-specific policy through inheritance. |
| Screen Map | A snapshot of the observations permitted for a particular conversation. Private observations are omitted; absent observations remain unknown. |
| Subject Property | A dated, typed observation with a value, unit, domain, source, confidence descriptor, and optional relationship-group identifier. |
| Covalent / linked nature | In this software, a nonzero relationship-group ID means only that a researcher has explicitly linked records for a stated analysis. It does not prove causality and is not a claim that two human traits are chemically covalent. Literal covalent bonds require chemical or structural evidence and belong in a molecular model. |
| Indices / gains | A change between two observations of the same property, in the same unit and domain, measured at different times. A change is descriptive; by itself it does not establish a cause or a durable gain. |
| Self-Assessment Snapshot | A person's own dated description of a concern, goal, strength, or experience. “Supreme Brow” is retained only as a conceptual nickname for this self-report; it is not an IQ or validated psychological test. |
| Positive Generationalism | A values-and-context concept for recording how people pass on skills, knowledge, resources, care, and opportunities across generations. Such gains are possible aims, not automatic consequences of age or ancestry. |
| Animal-model evidence | Evidence from a named species/model, such as a mouse study. It may inform hypotheses, but it is never silently converted into an observation about a human participant. |

## 3. Scientific and ethical boundaries

### 3.1 Age, adulthood, and maturity

Chronological age is a contextual fact, not a wisdom score. The system has no age-plus-adult-years formula: adding overlapping age quantities would not measure wisdom, and the program does not use age 23 (or any other fixed age) as a cognitive or maturity threshold. Adult status, legal capacity, and eligibility depend on the relevant jurisdiction and question; this module makes no legal determination.

A person's timeline may contain self-described milestones—education, work, caregiving, recovery, learning, or other life events. The module can preserve dated observations and changes, but it will not decide that a person is “still a teenager,” “aged badly,” or mature enough for a role.

### 3.2 Intelligence, wisdom, and psychological assessment

Values such as “300+ wisdom” or “4000+ IQ” are not accepted as scientific scales by this module. IQ tests have defined instruments, norms, score ranges, validity evidence, and qualified interpretation; a conversational model cannot invent a meaningful score by assigning a larger integer. A self-report or school qualification may be recorded as exactly that source, but neither is silently transformed into a general-intelligence result.

Any genuine psychological assessment must use an instrument validated for its purpose and population, standardized administration, appropriate norms, trained interpretation, and explicit limitations. The APA emphasizes the importance of appropriate test use, context, diversity considerations, and limits on interpretation.

### 3.3 Cognitive aging and long-term change

Cognitive abilities are not one scalar. Memory, attention, language, processing speed, learning, decision-making, and everyday function can change differently. The National Institute on Aging describes both commonly observed age-related changes and abilities that may remain stable or improve; its Baltimore Longitudinal Study of Aging notes that there is no single chronological timetable for human aging.

Accordingly:
- Do not infer cognitive capacity from age alone.
- Compare like with like: same task, unit, instrument, and relevant conditions.
- Keep missing observations unknown rather than filling them with guesses.
- Treat a two-point difference as a descriptive change, not proof of a biological or pharmacological cause.
- Do not use this module to diagnose cognitive decline or neurodegenerative disease.

### 3.4 Health, lice, pharmacology, and protein research

A head-lice observation may be stored as a health-context observation if the user chooses. CDC states that head lice are not related to cleanliness. The module therefore forbids using lice observations as a proxy for intelligence, hygiene, character, family worth, or social merit. Animals do not get or spread human head lice.

Pharmacology, protein development, senescence research, and other biological findings belong in a separate research-evidence record with species, sample, assay, date, method, and citation. A measurement of a protein or a result in a mouse is not a measurement of a person's intellect. Animal models can illuminate mechanisms, but translation to human outcomes has important limits.

### 3.5 Motor control and professional competence

The module can store task-specific motor observations (for example, task completion time, error count, or a validated motor assessment), with source and units. It cannot certify someone as a motor-control expert. Expertise or professional fitness requires an independently defined standard and appropriate evidence; a conversation summary is not a credential.

### 3.6 Race, identity, relationships, and fairness

Race, skin tone, ancestry, sex, disability, and other protected or sensitive identities are not score inputs. The phrase “Brown men” is not implemented as a demographic tier or threshold. The same observation rules apply to every age group and background; no demographic trait increases or reduces a person's score because there is no person score.

A spouse's or another person's private traits, needs, or history are not inferred from the primary user's record. Relationship information must be voluntarily supplied and appropriately authorized before it is included in a shared conversation view.

### 3.7 U.S. institutions and policy

A U.S. government agency's jurisdiction or benefit rules do not validate a psychological or biomedical scale. The State Department, Social Security Administration, and state authorities may be relevant sources for their own legal or administrative subjects; they are not interchangeable with peer-reviewed science, clinical standards, or psychometric validation. This software does not encode Social Security eligibility, retirement age, legal adulthood, or professional licensing.

## 4. Data model and provenance

Each observation stores a short stable key and unit; one finite numeric value; observation time (Unix seconds); a subject domain (context, learning, skill, wellbeing, biology, health, motor task, relationship, or research evidence); a source kind (self-report, direct observation, standardized instrument, qualified professional, peer-reviewed research, or animal model); a confidence descriptor from 0 to 1 supplied by the caller and not interpreted as a probability of truth; an optional relationship-group ID; and an explicit share-with-conversation flag.

The C API has a bounded capacity of 64 observations. It stores no name, address, account ID, or medical record number, performs no network calls, and writes no files. Applications decide persistence and access controls.

Animal-model observations are restricted to the research-evidence domain. The API does not allow an animal study to be entered as a human health or skill observation.

## 5. Integer reduction and ratio summaries

The C function sm_reduce_integer accepts one finite value, lower and upper integer limits, and a rounding rule (nearest, floor, ceil, or truncate). Nearest uses ties away from zero. Values outside the requested interval are clamped and the result reports that fact.

The C function sm_reduce_ratio computes numerator / denominator × scale, then applies the same bounded reduction. A zero denominator, NaN, infinity, or invalid range is rejected. The caller is responsible for selecting meaningful quantities and compatible units. The integer output is a compact representation of that input only.

## 6. Longitudinal comparison

sm_observation_delta compares two records only when their keys, units, and domains match and the second record has a later timestamp. It returns later.value - earlier.value. The caller must ensure the instrument and collection conditions are comparable. The library does not smooth data, fill gaps, infer causality, or label a change as improvement unless the task definition independently supports that interpretation.

## 7. Polymorphic Conversation Mask

The C API offers a deterministic consent-and-domain filter. The C++17 ConversationMask wraps that API and exposes a virtual permits() hook for inheritance. A derived mask can further narrow the visible domains, such as a learning-only view or a motor-task view. It cannot re-enable private observations filtered by the C API.

A screen map reports how many observations were omitted as private or outside the requested domains and sets unknowns_left_unknown. This makes absence explicit instead of generating a fictional property.

## 8. Build and test

From this directory:

    make
    make test
    ./build/example-c
    ./build/example-cpp

The Makefile is intentionally local to philosophy/. This first implementation does not change the repository's top-level build graph or assert that the main compiler/runtime automatically discovers the module.

## 9. References

1. American Psychological Association, [Guidelines for Psychological Assessment and Evaluation](https://www.apa.org/about/policy/guidelines-psychological-assessment-evaluation.pdf). Assessment, instrument fit, context, interpretation, and reporting.
2. American Psychological Association, [Ethical Principles of Psychologists and Code of Conduct, Standard 9](https://www.apa.org/ethics/code?item=8). Test construction, appropriate use, and interpretation limits.
3. National Institute on Aging, [How the Aging Brain Affects Thinking](https://www.nia.nih.gov/health/brain-health/how-aging-brain-affects-thinking). Cognitive domains and age-associated change.
4. National Institute on Aging, [Advances in Aging Research: The Baltimore Longitudinal Study of Aging](https://www.nia.nih.gov/research/labs/blsa/advances-aging-research). Individual differences and the absence of one chronological aging timetable.
5. Centers for Disease Control and Prevention, [About Head Lice](https://www.cdc.gov/lice/about/head-lice.html). Transmission and the statement that head lice are unrelated to cleanliness.
6. [Why animal model studies are lost in translation](https://pmc.ncbi.nlm.nih.gov/articles/PMC9052957/). Limitations when translating animal-model findings to human disease and treatment outcomes.

These references support the boundaries above; they do not validate this software as a medical, psychological, employment, or legal instrument.


---

# Event Tree and Social Listener

This is an explicit-input event store. It does not monitor ambient audio, television, networks, or other people.

## Core design

- AVL tree indexed by unique unsigned 64-bit event ID. Insert, find, and end-event updates are O(log n).
- Each event has an individual half-life in seconds. Relevance is 2^(-elapsed / half-life). An ended event starts its decay interval at the caller-supplied end timestamp.
- Callers choose shorter half-lives for transient status changes and longer half-lives for durable records. The code does not assume every marriage, resignation, diagnosis, or other life event has the same time constant.
- Pruning checks each event, so a full pruning pass is O(n), plus AVL rebalancing for removed nodes. The tree remains AVL-balanced; a pruning pass is not O(log n).
- Values preserve their kind: signed 64-bit integer or finite double, with declared property and unit.
- Provenance distinguishes self-report, direct observation, media/publication, assessor-authored text, and system import.
- A revisit can link to an earlier event using related_event_id. Repeating or replaying a thought is not independent corroboration.
- Medical requests and assessor-authored opinions are separate event types. An opinion requires supplied text, an author role, author attestation, and consent. The library cannot verify identity, licensure, or clinical correctness.
- Base, economic, and medical C++ listeners filter only explicitly submitted events consented for sharing.

## Neutral and evidence-aware listening

The library does not diagnose paranoia, label a person as paranoid, or infer that strangers, broadcasts, or unrelated events are directed at the listener. Neutral and evidence-aware modes are available. Evidence-aware views preserve source distinctions: first-person reports remain reports, external observations retain their stated source, and revisit links do not become corroboration. A person's self-report is not suppressed.

TV or broadcast information may be entered as a named media/publication source with a reference. It is not evidence that the broadcast is addressing or communicating with a particular person. Unknown intent remains unknown.

## Medical requests

A medical-request event records that the person asked for an assessor. An assessor-opinion event stores the assessor's own supplied statement and attestation. The library does not generate an assessor's opinion, verify a professional's identity or licensing, assess symptoms, or recommend medication. Host applications must implement authentication, authorization, encryption, audit, and professional verification before using this in a real medical workflow.

## Build

    make
    make test
    ./build/event-example-c
    ./build/event-example-cpp

The event tree is in-memory only. Persistence, encryption, authentication, and access controls belong to the host application.


### Hormone, medication, and media inputs

An integer or floating-point field can record a user-entered dose, published population statistic, industrial-growth metric, or other explicitly defined quantity. The value must retain its unit, timestamp, source, and study/context label. Recording a hormone or medication value does not establish that it improves intelligence, memory, or mood; causal or clinical conclusions require appropriate human evidence and a qualified assessor. Media observations describe the cited report, not a private message or targeted communication to the listener.


---

# Tree Consistency: Signal Toss and Branch Invariants

A cross-platform threaded monitor that continuously checks the event tree for
*branch consistency*. It is additive: the existing `event_tree` and
`social_listener` APIs are unchanged.

## Cadences

- **Sampling ~44 Hz.** A background worker wakes about 44 times per second and
  "tosses a signal" at a branch to check it. Each toss feeds the pure
  consistency monitor.
- **Committed updates 800–1200/hour.** A logged "tree is consistent" update is
  emitted only when a throttle interval has elapsed, keeping the confirmation
  rate inside the 800–1200/hour band (default 1000/hour). The two cadences are
  one mechanism: fast sampling, throttled confirmation.

## Update accounting

- **+4 every time a branch is considered.**
- **+5 every time a branch's length is considered while that branch is
  object-mature.**

So one consistent, mature branch accrues 9 updates per toss (`4 + 5`); an
immature or inconsistent branch accrues 4.

## Branch invariants (hard consistency rules)

| Rule | Bound |
|---|---|
| Traits per branch | ≤ 45 (soft warning at 44) |
| Branch weight | clamped to [1, 9] |
| Weight-9 branch that studies the economy | studies exactly 2 things |
| Branching points per side | ≤ 15 (12–15 typical); both sides considered for the total arrangement |

## Maturity by reference weighting

Maturity is probabilistic, not a fixed age. A branch supplies weighted
references, and the monitor computes a reference-weighted truth value with
three ascending gates:

- **0.80** — provisional maturity (candidate),
- **0.97** — strong maturity (confirmed),
- **0.51** — the decisive majority-of-references gate: *object is Mature* is
  declared **yes only when the reference-weighted truth is ≥ 0.51*, even after
  the 0.80 / 0.97 stages are reached.

## Portability

Threading is selected at compile time by `thread_compat.*`, preferring the
"best available" backend: ISO C11 `<threads.h>`, else Win32
`CreateThread`/`CRITICAL_SECTION` (covers Windows XP+, 7+, 10+), else POSIX
`pthreads` (Linux, macOS, BSD). The same source builds consistently on all of
them.

## Build and test

    make
    make test
    ./build/consistency-example

`test-consistency` includes a simulated-hour drive that asserts the committed
update count lands in [800, 1200] and that accrued updates equal
`samples × 9`, plus a live threaded smoke test.
