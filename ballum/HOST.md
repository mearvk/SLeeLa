# Ballum™ Program Host

**Package:** `ballum/`  **Date:** 2026-11-21

The Ballum program host puts a real program, such as a terminal or an admin
interface, inside the white Ballum environment. Ballum receives the program's
pixels and acts as its monitor: every key and mouse event arriving at Ballum is
replayed into the program 1:1, as if the program ran on a native X11 or Windows
GUI stack.

```
make host
build/ballum-host --list
build/ballum-host --profile sleela-terminal --enter disintegrate
build/ballum-host --profile xterm
build/ballum-host --enter slide-up -- /usr/bin/xterm -e top
```

Leave a session with **Ctrl+Alt+Esc**. Closing the program also ends it.

## How the pixels are exact

A terminal draws itself with its real toolkit, fonts and colors; re-implementing
a terminal inside Ballum would only approximate it. So Ballum does not render
the program. It runs the program on its **own private X display** (Xvfb) and
captures that display's pixels:

```
program ──draws──► private X display ──capture──► host ──blit──► Ballum surface
program ◄─XTEST──  private X display ◄──router──  host ◄─events─ Ballum surface
```

The capture is the program's own framebuffer, so the result is pixel-for-pixel
what the program shows on a native desktop. There is one conversion step,
BGRX → RGBA, and one memcmp per frame to skip frames that did not change.

Because the display is private, the program cannot see, capture or inject into
the user's real desktop, and the XTEST input reaches only the private display.

## Layout

```
+--------------------------------------------------------------+
| APP NAME slot                          | DATE TIME slot      |  header
+--------+-----------------------------------------+-----------+
| LEFT   |                                         | RIGHT     |
| sibling|            PROGRAM VIEWPORT             | sibling   |
+--------+-----------------------------------------+-----------+
|                      BOTTOM sibling                          |
+--------------------------------------------------------------+
```

Everything is exact integer pixels (`ballum_host_layout`). The program is
started at exactly the viewport size and its first window is pinned to it.

### APP NAME and DATE TIME components

The two header slots sit above the program. **The components themselves do not
exist yet.** A repository check (2026-11-21) found:

| Need | In the repository today |
|---|---|
| App Name component | none. `lib/user-interface/SLStatusBar` is a footer strip and `lib/ui/SLLabel` is a generic label; neither is a Ballum slot component |
| Date Time component | none. `lib/core/SLClock` provides clock access but no drawing component |
| Text drawing inside the Ballum framebuffer | none. Ballum is a pixel/blit surface with no font engine (`ballum/example` embeds a tiny 5×7 font) |

So the host reserves and clears both slots and offers a stable contract for the
components to be built against:

```c
static void draw_app_name(void *user, ballum_surface_view_t *view) { /* draw into view */ }

ballum_component_t name = { draw_app_name, NULL /* optional input */, user, 0 };
ballum_component_t clock = { draw_clock,   NULL, user, 1000 };   /* redraw every second */
ballum_host_set_slot(host, BALLUM_SLOT_APP_NAME, &name);
ballum_host_set_slot(host, BALLUM_SLOT_DATE_TIME, &clock);
```

A component receives a `ballum_surface_view_t` covering exactly its rectangle;
`ballum_view_put` / `ballum_view_fill` clip to it, so a component cannot draw
outside its slot. `refresh_ms` makes the host redraw the slot at that interval
(the date-time clock); `0` draws on layout change only. Starter profiles carry a
`title` for the app-name component to show.

Sibling components attach the same way with `ballum_host_add_sibling` before
the program starts (the viewport size is then final).

## Input: 1:1, with an escape hatch

| Input | Forwarded as |
|---|---|
| pointer motion | position in viewport coordinates (display minus viewport origin) |
| buttons 1–3, wheel 4–7 | button press/release, each preceded by the position |
| keys | symbol + physical code, press and release |
| press inside the viewport, drag outside | still forwarded, clamped to the viewport, until release |
| events over the header or a sibling | delivered to that component only, in local coordinates |

Input arrives from the Ballum surface with `XGrabKeyboard` / `XGrabPointer`
(revocable, and a refused grab is tolerated unless `require_grab` is set).
Keys are looked up by symbol in the program display's own keymap; the physical
code is the fallback. Key repeat arrives as release+press pairs, as the X server
reports it.

**Remapping.** `ballum_host_remap_key(host, from, required_modifiers, to)` and
`ballum_host_remap_button(host, from, to)` rewrite or drop (`to == 0`) input
before it is routed. The escape chord cannot be remapped away.

**Escape chord.** Ctrl+Alt+Esc is consumed by the host and never reaches the
program, so a bare Esc still belongs to vim or a shell. The chord is checked
every step, independently of the program, so a hung program cannot trap the
user.

## Entry and exit effects

`ballum_transition.h` works on whole frames, so effects can move the program
rather than only mask it. Names for `--enter` / `--leave`:

| Name | Effect |
|---|---|
| `disintegrate` | every pixel flies in from a scattered position and settles; played backwards it is the disintegration |
| `slide-left` `slide-right` `slide-up` `slide-down` | the program frame slides in from that edge |
| `pixelize` | a coarse mosaic of the frame sharpens to full resolution |
| `none` | the program appears at once |
| `fade` `wipe-*` `iris-in` `iris-out` `bars` `blinds` `dissolve` `ripple` `checker` `diamond` `clock` `sparkle` `zoom` `spotlight` … | the 20-effect catalog of `ballum_effects.h` |

Contract: at progress 100 the output is the program frame exactly; at 0 it is
the background. Exit plays the same effect backwards (`100 − t`). Integer-only
and deterministic.

## Starter profiles

`build/ballum-host --list` shows them; `ballum_host_profiles.h` is the data.

| Profile | Runs | Enter / leave |
|---|---|---|
| `sleela-terminal` | `SleelaTerminal` (GTK/VTE, slsh) | disintegrate / fade |
| `slsh` | `xterm -e slsh` | slide-up / slide-down |
| `xterm` | light xterm | pixelize / fade |
| `xterm-dark` | dark xterm | fade / fade |
| `top` | `xterm -e top`, a small admin view | iris-in / iris-out |

## Security

Aligned with NIST SP 800-53 Rev. 5 controls **AC-6** (least privilege),
**SC-39** (process isolation), **SI-10** (input validation), **CM-7** (least
functionality) and **SC-7** (no network listener). This describes design intent;
the code has not been assessed or certified against any standard.

| Measure | Where |
|---|---|
| program runs `no_new_privs`, with a seccomp-BPF denylist (ptrace, process_vm_*, mount family, module loading, kexec, reboot, bpf, perf_event_open, userfaultfd, keyrings, clock/hostname setters, io_uring, setns); foreign syscall ABIs kill the process | `ballum_sandbox.c` |
| no core dumps, bounded open files, clean allowlisted environment, all inherited descriptors closed, own session and process group, killed if Ballum dies | `ballum_sandbox.c` |
| Ballum marks itself non-dumpable so other processes cannot read the input it monitors | `ballum_sandbox_harden_self` |
| programs resolved only against a fixed trusted path; group/world-writable files and directories rejected (CWE-426); no shell is ever involved, so no injection through arguments | `ballum_sandbox_resolve` |
| private X display over a 0600 `MIT-MAGIC-COOKIE-1` file in a 0700 directory, no TCP | `ballum_host_x11.c` |
| every protection fails closed: if it cannot be established, the program is not started | all |
| host refuses to start without full keyboard and pointer input, so the user can always leave | `ballum_host_create` |
| bounded input queues, validated rectangles and sizes, frames copied into host-owned memory | `ballum_host.c`, X11 backend |

**Limits, stated plainly.** The seccomp layer is a denylist, not a container:
the program still sees the user's files and can run as the user. It limits what
a misbehaving program can do to the system; it is not a boundary against
hostile code. Isolating the filesystem needs namespaces or a separate account
and is future work. `XDG_RUNTIME_DIR` is passed through so toolkits can reach
their settings service. A program that is a launcher forking and exiting is
treated as ended. Windows and macOS have no program source yet and report
`UNSUPPORTED`; Wayland needs a different input path.

## Requirements and verification

Run time (Linux): an `Xvfb` binary and `libXtst.so.6` on the machine, plus the
program. Build time: libX11 (and Xrandr for the surface backend).

| Check | Command | Needs a display |
|---|---|---|
| layout, lifecycle, transitions, routing, remap, components, fail-closed paths | `make test` | no |
| process protection, including seccomp denial surviving `exec` | `make test` | no |
| capture and 1:1 input against a real X client | `make test-x11` | Xvfb, libXtst, xterm |
| an actual session on screen | `make host && build/ballum-host --profile xterm` | an X11 session |

`make test` is portable and passes without any display libraries.
`make test-x11` and the on-screen session were **not run** when this was
written: the development sandbox had no X server and no package access. The
source compiles and its failure paths (missing Xvfb) were exercised; the capture
and input forwarding have not yet run against a live server. Run
`make test-x11` on a machine with Xvfb and xterm before relying on them.

## Files

| File | Role |
|---|---|
| `include/sleela_ballum/ballum_host.h`, `src/ballum_host.c` | compositor, layout, state machine, input router, remap, components |
| `include/sleela_ballum/ballum_transition.h`, `src/ballum_transition.c` | entry/exit transitions |
| `include/sleela_ballum/ballum_sandbox.h`, `src/ballum_sandbox.c` | process protection |
| `src/ballum_host_x11.c` | private-display source (Xvfb + capture + XTEST) |
| `include/sleela_ballum/ballum_host_profiles.h`, `src/ballum_host_profiles.c` | starter profiles |
| `src/ballum_host_cli.c` | `ballum-host` |
| `src/ballum_backend_x11.c` (extended) | event pump, full key events, wheel, input grab, focus |

Backend additions in `ballum_manager.h`: `ballum_key_event_t`,
`poll_key_event`, `set_input_grab`, and wheel buttons 4–7. All are additive;
existing backends leave the new slots NULL and the manager reports
`UNSUPPORTED`.
