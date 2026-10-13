# SLeeLa `lib/gui` — events, DHTML-style DOM, and scripting

**Revision:** 0.1 · **Family:** `gui`

This is the **backing API** for dynamic SleelaUI interfaces: action callbacks /
`onClick`, a browser-DOM-style document model with live manipulation, and a
small scripting engine that drives it. It extends the original `lib/gui`
integration layer (window / action / 1..14 document watch) without changing its
posture: **SLeeLa owns the meaning of events and owns the document; the host
raises activations and mirrors the DOM onto real SleelaUI widgets.**

## The three sub-layers

### 1. Events & callbacks

| Class | Role |
|---|---|
| `SLGuiEventType` | The event catalogue (`click`, `input`, `change`, `keydown`, `pointerdown`, `submit`, `toggle`, `custom`, …) with stable ordinals and canonical names. |
| `SLGuiEvent` | One dispatched event: type, target id, value/key/pointer payload, plus `preventDefault()` and `stopPropagation()` (exactly like a browser event). |
| `SLGuiHandler` | One registered callback: `(type, target, operation)`, with `once`/`capture`, and the take-the-latch activation `SLGuiAction` uses. |
| `SLGuiEventBus` | The dispatcher: `addListener`/`removeListener`, phase-ordered `dispatch` (capture → target/bubble), `stopPropagation` honoring, `once` sweeping. |

### 2. DHTML-style DOM

| Class | Role |
|---|---|
| `SLGuiNode` | A DOM element: `tag`, `id`, text, parent/children, an attribute map, an inline **style** map, visibility/enabled, and an optional live widget handle. |
| `SLGuiDom` | The document: `createElement`, `getElementById`, `appendChild`, `removeChild`, `setText`/`setAttribute`/`setStyle`, `show`/`hide`, `getElementsByTag`, `querySelectorByAttr`, and `addEventListener` + `onClick`/`onInput`/`onChange`/`onKeyDown`/`onSubmit`. Owns an `SLGuiEventBus`. |

### 3. Scripting

| Class | Role |
|---|---|
| `SLGuiScriptCommand` | One instruction: a verb (`create`, `append`, `text`, `attr`, `style`, `show`, `hide`, `enable`, `on`, `fire`, `call`, `remove`, `nop`) + up to four operands. |
| `SLGuiScript` | The engine: an ordered command list run against an `SLGuiDom`, with the `call` verb crossing the native boundary via `SLGuiBridge`. **Deliberately not a general interpreter** — a bounded, auditable verb list that can do no more than the DOM API already allows. |

## How a callback flows

```
host raises activation ─▶ SLGuiEvent(type, target, value)
                          │
                          ▼
          SLGuiDom.dispatchEvent ─▶ SLGuiEventBus.dispatch
                          │  (capture → target/bubble, stopPropagation-aware)
                          ▼
          matching SLGuiHandlers fire ─▶ bound SLeeLa operation
                          │
                          ▼
      SLeeLa mutates the DOM (setText / setStyle / hide / …)
                          │
                          ▼
      host walks dirty SLGuiNodes ─▶ updates live SleelaUI widgets
```

## Minimal usage

```sleela
SLGuiDom dom = new SLGuiDom(); dom.init();
dom.createRoot("window", "app");
SLGuiNode save = dom.appendElement("app", "button", "save");
save.setText("Save");

dom.onClick("save", "doSave");          // bind a callback
dom.fire(SLGuiEventType.CLICK, "save", ""); // host raises; dispatcher routes it
```

Scripted equivalent:

```sleela
SLGuiScript s = new SLGuiScript(); s.init(dom);
s.create("button", "save", "app");
s.text("save", "Save");
s.on("save", "click", "doSave");
s.fire("save", "click", "");
s.run();
```

A runnable walkthrough of all three sub-layers is
[`gui-events-demo.sleela`](gui-events-demo.sleela).

## Relationship to the toolkit

The DOM nodes name SleelaUI concepts (`window`, `box`, `button`, `label`,
`entry`, …). The host mounts each node onto the matching
[`/lib/user-interface`](../user-interface/) widget and pushes dirty-node changes
to it; conversely it raises `SLGuiEvent`s when the user interacts. The event
bus and DOM stay the single SLeeLa-owned source of truth, consistent with the
`lib/gui` division of authority.

**SLeeLa — MEARVK LLC — 2026**
