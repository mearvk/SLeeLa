# `observer` — the dynamic document observer

A standing document is watched by an observer. Modules are admitted to the
observer **while the program is running**; when the document changes every
active module is told, and may answer, converse with other modules, or
**propose** an edit. The observer is the only writer of the document.

## Classes

| Class | Role |
|---|---|
| `SLObserverDocument` | The standing document: up to 64 lines, a revision that moves by exactly 1 per accepted edit. Refused edits change nothing. |
| `SLObserverMessage` | The one envelope: NOTE, QUESTION, ANSWER, PROPOSE, FAULT, CHANGE. |
| `SLObserverPort` | A module's bounded mailbox (16 in, 8 out per cycle). |
| `SLDocumentObserver` | The hub. Nested built-in module `SLDocumentObserver.Auditor`. |
| `SLObserverSummaryModule` | Drop-in; nested `Responder` tracks revision and edit mix, answers `summary?`. |
| `SLObserverGuardModule` | Drop-in; nested `Responder` proposes redacting a forbidden line. |
| `SLObserverReviewModule` | Drop-in; nested `Responder` opens a conversation with the auditor. |
| `SLObserverView` | The modest GUI: a retained widget tree (labels, entry, buttons) with take-the-latch button presses. |
| `observer-demo` | Self-checking scenario (39 checks). |
| `observer-gui-demo` | Scripted presses through the view. |

```
sleela run lib/observer/observer-demo.sleela
sleela run lib/observer/observer-gui-demo.sleela
```

## Dropping a module in

A module is a nested class holding an `SLObserverPort`:

```
SLObserverSummaryModule.Responder m = new SLObserverSummaryModule.Responder();
m.attach("summary");
observer.admit(m.portOf());     // at any time between cycles
```

then the host cycle pumps it:

```
observer.edit(0, line, "text", "host");      // delivers a CHANGE to every active port
while (observer.beginCycle()) {
  m.pump();                                   // each module reads its inbox, writes its outbox
  observer.endCycle();                        // the observer routes what was said
}
```

### Honest limit: what "while running" means

SLeeLa links classes when a program starts; there is no run-time class loader
in the VM. What is dynamic here is **admission**: new module instances (of
nested classes already linked) join and leave a live observer at any time, with
no restart and no change to the others. Linking a module whose source did not
exist at start would need a host that restarts or re-links the program.

The language also does not accept a subclass where its base is expected
(`extends` does not widen types), so there is no shared `Responder` interface.
Instead each module exposes `portOf()` and `pump()`, and the host's cycle calls
`pump()` on the modules it holds.

## Safety envelope (all enforced by the observer)

| Rule | Limit |
|---|---|
| Modules never write the document; they **propose** | applied only if based on the current revision (optimistic concurrency) |
| Proposals per cycle | 2 |
| Proposal depth per host edit | 3 (a stubborn module cannot loop forever) |
| Conversation hops | 4 (the next message is dropped and audited) |
| Cycles per host edit | 16 (leftover mail is discarded and audited) |
| Faults | 3 reported faults or outbox overflows quarantine a module; the rest carry on |
| Sender | stamped by the observer; a module cannot impersonate another |
| Admission | refused mid-cycle, when full (14), for empty/reserved/duplicate names, or non-fresh ports |
| Edits | out-of-range, unknown op, or null text are refused with no change |
| Audit | every admit/edit/decline/drop/quarantine goes to a bounded trail (64) |

## Conversation

A module answers a QUESTION with `port.reply(...)`; `observer` is told of
declined or accepted proposals through an ANSWER whose `extra` is `"declined"`
or `"accepted"`. `SLObserverReviewModule` shows a chain (question, answer,
follow-up) that the hop limit finally ends.

## Authoring notes

`route` is a reserved word in SLeeLa (it is a contextual built-in), so the
observer's routing method is `deliverMessage`. `SLGuiDom`/`SLGuiApp` do not link
under the current binary (pre-existing: `SLGuiEventType` statics and an
`SLTheme` ambiguity between `ui` and `user-interface`), so the view is
self-contained rather than built on them.
