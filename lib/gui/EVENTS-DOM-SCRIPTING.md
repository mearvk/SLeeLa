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
[`lib/gui/version-mual/gui-events-demo.sleela`](lib/gui/version-mual/gui-events-demo.sleela)
(the GUI `.sleela` impl moved to `lib/gui/version-mual/`, copied to `lib/gui/version-gual/`).

## The support structure (DOM ↔ toolkit wiring)

The event/DOM/scripting layer is SLeeLa-owned and host-agnostic. The **support
structure** is the concrete wiring that projects it onto real
[`/lib/user-interface`](../user-interface/) widgets and feeds interactions back:

| Class | Role |
|---|---|
| `SLGuiWidgetFactory` | The one place that maps a node `tag` → a SleelaUI constructor (`uiBox`/`uiButton`/`uiLabel`/`uiEntry`/…) and says which tags are interactive and what event they raise. |
| `SLGuiMount` | The **reconciler**: mounts the DOM depth-first (parent before child), keeps an `id → widget-handle` map, and on each pass applies every **dirty** node's text/visibility/sensitivity/accent/size to its widget, then clears the flag. The SLeeLa→toolkit half. |
| `SLGuiHostAdapter` | Polls each mounted interactive widget's activation latch (`uiWidgetTakeActivated`) and raises the matching `SLGuiEvent` (CLICK/SUBMIT/TOGGLE, carrying the widget's live value) into the bus. The toolkit→SLeeLa half. |
| `SLGuiApp` | The top-level object: opens the app + window, owns the DOM + mount + host adapter + script engine, and runs the loop (**pump → poll activations → mount new nodes → reconcile**). |

```
            SLeeLa-owned                         host / toolkit
         ┌───────────────┐   reconcile (dirty)  ┌──────────────────┐
         │   SLGuiDom     │ ───── SLGuiMount ───▶ │ SleelaUI widgets │
         │  (SLGuiNode)   │                       │ (ui* built-ins)  │
         │   SLGuiEventBus │ ◀─── SLGuiHostAdapter ─ (uiWidgetTake-  │
         └───────────────┘    dispatch events     │   Activated)     │
                 ▲                                 └──────────────────┘
                 │ SLGuiApp owns + runs the loop
```

### Minimal support-structure usage

```sleela
SLGuiApp app = new SLGuiApp();
if (app.boot("com.mearvk.App", "App", 640, 420)) {
  SLGuiNode save = app.add("root", "button", "save");
  save.setText("Save");
  app.onClick("save", "doSave");   // bind a callback
  app.run();                       // mount, show, loop (poll + reconcile)
}
```

A runnable walkthrough is
[`lib/gui/version-mual/gui-app-demo.sleela`](lib/gui/version-mual/gui-app-demo.sleela).

## Relationship to the toolkit

The DOM nodes name SleelaUI concepts (`window`, `box`, `button`, `label`,
`entry`, …). `SLGuiMount` mounts each node onto the matching
[`/lib/user-interface`](../user-interface/) widget and pushes dirty-node changes
to it; `SLGuiHostAdapter` raises `SLGuiEvent`s when the user interacts. The
event bus and DOM stay the single SLeeLa-owned source of truth, consistent with
the `lib/gui` division of authority.

**SLeeLa — MEARVK LLC — 2026**
