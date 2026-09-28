# systems/xml — BODI™ System Definitions

**Author:** Max Rupplin — MEARVK LLC — 2026
**Trademarks:** BODI™ (Bloq Object Directory Interface), Wrapper™ (`.sleela`).

This directory holds **BODI™ system definitions**: XML source artifacts that the
SLeeLa toolchain lowers into runnable forms. A system definition is not itself
executed — it *declares* an addressable system so that every emitted form stays
answerable to one witnessed contract. This follows the existing "system XML"
`<document><container mode purpose>` style used elsewhere in the repository
(e.g. `src/implementations/_001_/nordshrift/drivers/nordshrift.system.xml`) and
the BODI™ semantic model in [`../../BODI.md`](../../BODI.md).

## Files

| File | System | Emits | Purpose |
|---|---|---|---|
| [`sleela-gui.system.xml`](sleela-gui.system.xml) | `sleela.gui` | **Java** + **SLeeLa** (Wrapper™) | The complete SLeeLa GUI surface, compared against the Java 28 SecureJDK memory host. |

## `sleela-gui.system.xml`

The GUI system definition captures **all the GUI detail** in one place and
compiles to two congruent outputs:

- **Java** — the presentation host under `com.mearvk.sleela.gui`: the
  toolkit-neutral `SleelaGui` contract, the **Swing** backend
  (`JFrame`/`JPanel`/`JLabel`/`JButton`, `BorderLayout`, Event Dispatch Thread)
  and the **JavaFX** backend (`Stage`/`Scene`/`VBox`/`Label`/`Button`, the
  JavaFX Application Thread), plus the host adapters (`SleelaGuiHost`,
  `SleelaGuiRuntime`, `SleelaRuntime`) and the native bridge facade.
- **SLeeLa** — a Wrapper™ (`#sleela 1.2`) that owns **GUI intent**: window,
  text, and named actions expressed as SLeeLa operations that Java realizes.

The two emits are held **congruent** (same components, events, and action
names); a name in one but not the other stops at the BODI™ boundary rather than
being bridged.

### Compared to Java 28

The document's runtime container is the "GUI compared to Java 28" surface. It
records the **Java 28 SecureJDK** memory link (series **J28-MEM-0001**, Model A:
SLeeLa drives, Java 28 owns the objects):

- the `SLValue` ↔ Java 28 **value contract** (`n/i/d/b/s` values and the opaque
  object `h` **handle**);
- the **operation set** (`hello`, `new`, `call`, `get`, `set`, `free`,
  `describe`, `stats`, `bye`);
- the two interchangeable **channels** (JNI local feedback and loopback port)
  funneled through `SleelaMemoryServer.handleLine`;
- the **SecureJDK posture** (class allow-list, handle isolation, no DTD / no
  wire code loading, bounded live handles).

The GUI therefore never holds a Java object — it holds a handle and drives it by
value, so a button click can update durable Java 28 state and publish the reply
back into the window label.

## BODI™ structure

The document is organized as three BODI™ lifecycle containers inside the
**mitigative circumference**:

| Mode | Purpose | Contents |
|---|---|---|
| `1` | `startup` | Declare the GUI system, components, events, threading, backends, and the three integration paths; emit Java + SLeeLa. |
| `2` | `runtime` | The live event circumference and the Java 28 SecureJDK memory bridge. |
| `3` | `shutdown` | Orderly teardown: close window → free handles → close runtime → commit witness. |

Every emit, event, and memory operation declares its BODI™ address, verb,
sequence, witness, and boundary. See the **System Preface** at the top of the
XML for the fixed reading order.

## References

- [`../../BODI.md`](../../BODI.md) — the BODI™ witness model and middle-verb set.
- [`../../gui/README.md`](../../gui/README.md), [`../../gui/INTEGRATION.md`](../../gui/INTEGRATION.md) — the GUI library and its three integration paths.
- [`../../java28/spec/JAVA28-MEMORY-INTEGRATION.md`](../../java28/spec/JAVA28-MEMORY-INTEGRATION.md) — the J28-MEM-0001 memory contract.
- [`../../COMPILER.md`](../../COMPILER.md), [`../../SLEELA.md`](../../SLEELA.md) — the Sleela compiler and the Wrapper™ filetype.
