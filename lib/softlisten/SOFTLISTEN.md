# `softlisten` — soft offers and soft listeners

People **offer** things to a running system: a document edit, an observer
module, a vertical-integration tier. The consuming model has not agreed to them
yet — maybe they are malicious, maybe they claim ties that do not exist, maybe
the loader would refuse them today. A **soft offer** waits at a desk, changes
nothing, and is ascertained by **soft listeners** until the consuming model
agrees, declines, or the offerer withdraws.

```
sleela run lib/softlisten/softlisten-demo.sleela     # 81 self-checks
```

## The lifecycle of an offer

```
begin() ──▶ fill in ──▶ submit() ──▶ assessed (waiting) ──▶ agree()    ──▶ applied through the observer / linker
                                          │  ▲                decline()  ──▶ closed
                                          │  └── reassess()   withdraw() ──▶ closed (offerer only)
                                          └── a document change re-checks every open offer (the desk is a listener)
```

`agree()` always **re-assesses first**, then applies. Nothing an offer says
about itself is trusted at agreement time.

| Offer kind | Applied by agreement as |
|---|---|
| 0 insert / 1 replace / 2 delete a line | `SLDocumentObserver.edit(...)` (the observer can still refuse) |
| 3 module | `SLHotLinker.attachModule(...)`, then each claimed tie that names a live instance is linked for real; a failed tie detaches the instance |
| 4 tier | `SLHotLinkIntegration.expand(...)`, then its ties |

## Findings and severity

| Severity | Meaning | Agreement |
|---|---|---|
| 0 info | something ran and was fine (`preflight-ok`) | allowed |
| 1 warn | suspicious but not disqualifying | only if the agreer acknowledges warnings |
| 2 **hold** | not agreeable **yet**; may clear when the world changes | refused |
| 3 **block** | never | refused |

A hold is the "soft" part: the offer is neither good nor bad, it is not agreed
*for some reason*, and the reason is recorded. Examples of holds that clear:
a tie to an anchor the host has not registered yet, a full instance cap, a
conflict with a line that has since changed, an unverified type the host has
not permitted. A block does not clear.

## The soft listeners (nested in `SLSoftOfferDesk`)

| Listener | Ascertains | Findings |
|---|---|---|
| **Malice** | content that should not be agreed | `deny-line`, `deny-token`, `reserved-offerer`, `empty-text`, `duplicate-offer`, `offer-flood`, `tokens-clipped` |
| **Ties** | false ties | `false-tie` (names nothing known, hold), `dead-tie` (names a detached instance, block), `undeclared-tie` (the hot-link matrix does not declare that connection, block), `self-tie`, `duplicate-tie`, `empty-tie`, `ties-clipped` |
| **Loader** | loader issues, by a **dry run** of `SLHotLinker.preflight` | `unknown-type`, `wrong-contract`, `no-type-for-kind`, `no-port`, `port-name-mismatch`, `port-not-fresh`, `loader-permanent` (block), `loader-transient` (hold), `stack-full`, `preflight-ok` |
| *use-model* (the desk itself) | fit with the live document | `conflict` (the line changed since the offer was made), `out-of-range`, `no-text` |

`preflight` is the same code `attach` runs, so the dry run cannot disagree with
the real thing. It classifies a refusal as *transient* (cap reached, authorizer
or digest not yet provided, unverified type not yet permitted) or *permanent*.

## Soft listeners have a lifecycle too

Each can be **paused**, **resumed**, **altered** (level) and **retired**
(final), like observer listeners:

| Level | Effect |
|---|---|
| 0 lenient | a block is downgraded to a hold |
| 1 normal | as declared |
| 2 strict | a warn is upgraded to a hold |

**Fail closed:** while a soft listener is paused or retired, every assessment
carries a `listener-unavailable` hold, so an offer can never look clean merely
because its checker was switched off. The deny lists can be extended at run time
(`addDenyToken`, `addDenyLine`) and take effect on the next assessment.

## Desk safety

* An offer must be issued by `begin()` and can be lodged once.
* At most 4 open offers per offerer, 32 offers on the desk (`compact()` frees closed ones).
* Module and tier offers need an executive agreer (trust 2); any agreement needs trust 1.
* Only the offerer can withdraw. Reserved offerer names are blocked.
* Findings are capped at 16 per offer; **an overflow counts as a block**.
* Every lodge, refusal, agreement and listener change is audited (64 entries).

## Honest limits

* **No free-text analysis.** SLeeLa `String` has no substring or character
  access, so the listeners compare whole lines and whole tokens by exact
  (case-sensitive) equality. The host splits text into tokens; only the first 8
  are inspected and any further tokens are reported as a hold, never ignored.
* **The deny lists are a seed, not a virus database.** For real scanning use the
  `antivirus` package's native engine; this layer decides what to do with the
  verdict.
* **Offerer trust is supplied by the host.** The desk does not authenticate
  anyone; it enforces what it is told.
* **Native-code offers** (`native.extension`) are not offerable as modules: their
  contract is `native-abi`, so the Loader blocks them as `wrong-contract`. The
  linker's authorizer and pinned-digest gates still apply to the host's own attach.
* The desk's `pump()` must be called in the host's observer cycle, like any module.
