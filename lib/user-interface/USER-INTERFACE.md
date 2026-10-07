# `lib/user-interface` — Building a Graphical Interface from SLeeLa

This package lets a SLeeLa program open a **real native window** and build a
graphical user interface — on Windows, macOS, or Linux/Unix — using **SleelaUI™**,
SLeeLa's **own, original** cross-platform UI toolkit (not GTK, Qt, or a wrapper).
A developer writes `ui.openWindow(...)`, `box.vertical(...)`, `button.createIn(...)`,
and the SLeeLa VM makes the **genuine SleelaUI C ABI call** for whatever OS the
program is running on:

```
 SLeeLa source (lib/user-interface/SL*.sleela)
   -> ui* built-ins               uiAppCreate / uiWindowCreate / uiButton / ...
     -> SLVM opcodes              OP_UI_* (sleela_core.c dispatch)
       -> SleelaUI C ABI          include/sleela_ui.h  (user-interface/)
         -> native backend        X11 (Xlib) / Cocoa (AppKit) / Win32 (GDI)
```

The same `.sleela` program is portable: it calls one named built-in and the VM
selects the X11, Cocoa, or Win32 backend at the native layer. The toolkit draws
its **own** widgets with its own software rasterizer, so the signature **Slick
Black** look is pixel-identical on all three desktops. The native toolkit, its
backends, theme, and documentation live under [`/user-interface`](../../user-interface/).

> **Boundary — honesty note.** These calls cross the explicit VM/UI bridge the
> same way the rest of the standard library does (sockets, files, time, the
> `os*` bridge in `/lib/os`). A window, widget, theme, or application is a
> **VM-local bounded handle**, never a raw `HWND`, `NSWindow*`, X11 `Window`, or
> C++ pointer — the same safety discipline used everywhere in the VM. The `ui*`
> built-ins require `#sleela 1.5`.

## The SLVM UI built-in surface

The VM exposes these built-ins (lowered to `OP_UI_*` opcodes, serviced by the
SleelaUI C ABI in `user-interface/include/sleela_ui.h`). Every backend
implements the same surface.

| Built-in | Effect | SleelaUI C ABI |
|---|---|---|
| `uiBackend()` → int | Live backend id (1 Win32 / 2 Cocoa / 3 X11) | `slui_backend` |
| `uiBackendName(id)` → String | `"win32"`/`"cocoa"`/`"x11"` | `slui_backend_name` |
| `uiVersion()` → String | Toolkit version string | `slui_version_string` |
| `uiAppCreate(id)` → int | Open the application; VM-local handle (`-1` fail) | `slui_app_create` |
| `uiAppDestroy(app)` | Release the application and its windows | `slui_app_destroy` |
| `uiAppPump(app,block)` → int | Dispatch pending events once | `slui_app_pump` |
| `uiAppRun(app)` → int | Run the event loop; return the exit code | `slui_app_run` |
| `uiAppQuit(app,code)` | Ask the loop to stop | `slui_app_quit` |
| `uiWindowCreate(app,title,w,h,theme)` → int | Open a window; VM-local handle | `slui_window_create` |
| `uiWindowShow/Hide(win)` | Map / unmap the window | `slui_window_show`/`_hide` |
| `uiWindowSetTitle(win,title)` | Set the title | `slui_window_set_title` |
| `uiWindowRoot(win)` → int | The root container widget handle | `slui_window_root` |
| `uiWindowSetTheme(win,theme)` | Swap the palette live | `slui_window_set_theme` |
| `uiWindowRedraw(win)` | Request a repaint | `slui_window_request_redraw` |
| `uiBox(parent,orient,spacing)` → int | A vertical/horizontal box | `slui_box` |
| `uiHeaderBar(parent)` → int | A title-bar strip | `slui_header_bar` |
| `uiLabel(parent,text)` → int | A text label | `slui_label` |
| `uiButton(parent,text)` → int | A push button | `slui_button` |
| `uiToggle(parent,text,on)` → int | A toggle switch | `slui_toggle` |
| `uiEntry(parent,placeholder)` → int | A single-line text field | `slui_entry` |
| `uiSlider(parent,min,max,val)` → int | A slider | `slui_slider` |
| `uiSeparator(parent,orient)` → int | A hairline rule | `slui_separator` |
| `uiSpacer(parent)` → int | A flexible expanding gap | `slui_spacer` |
| `uiCheckBox(parent,text,on)` → int | A labelled check box | `slui_check_box` |
| `uiRadioButton(parent,text,group,on)` → int | A grouped radio button | `slui_radio_button` |
| `uiComboBox(parent)` → int | A drop-down selection | `slui_combo_box` |
| `uiComboBoxAdd(combo,option)` | Append a combo option | `slui_combo_box_add` |
| `uiSpinButton(parent,min,max,step,val)` → int | A numeric spin button | `slui_spin_button` |
| `uiProgressBar(parent,frac)` → int | A determinate progress bar | `slui_progress_bar` |
| `uiLevelBar(parent,frac)` → int | A segmented level bar | `slui_level_bar` |
| `uiSpinner(parent)` → int | An indeterminate activity spinner | `slui_spinner` |
| `uiScrollBar(parent,orient,val,page)` → int | A scroll bar indicator | `slui_scroll_bar` |
| `uiFrame(parent,title)` → int | A titled bordered container | `slui_frame` |
| `uiCard(parent)` → int | A raised surface panel | `slui_card` |
| `uiGrid(parent,cols,spacing)` → int | A row/column grid | `slui_grid` |
| `uiStatusBar(parent)` → int | A footer status strip | `slui_status_bar` |
| `uiImage(parent,glyph,w,h)` → int | An image/icon tile | `slui_image` |
| `uiAvatar(parent,initial,d)` → int | A round avatar | `slui_avatar` |
| `uiBadge(parent,text)` → int | A count/status pill | `slui_badge` |
| `uiChip(parent,text)` → int | A tag / chip pill | `slui_chip` |
| `uiHeading(parent,text,size)` → int | A large title label | `slui_heading` |
| `uiTooltip(parent,text)` → int | A tooltip bubble | `slui_tooltip` |
| `uiLinkButton(parent,text)` → int | A hyperlink-style button | `slui_link_button` |
| `uiSearchEntry(parent,ph)` → int | A search field | `slui_search_entry` |
| `uiPasswordEntry(parent,ph)` → int | A masked password field | `slui_password_entry` |
| `uiInfoBar(parent,text,severity)` → int | An inline notice bar | `slui_info_bar` |
| `uiWidgetMargin(w,t,r,b,l)` | Set margins | `slui_widget_set_margin` |
| `uiWidgetAlign(w,h,v)` | Set alignment | `slui_widget_set_align` |
| `uiWidgetExpand(w,h,v)` | Set expansion | `slui_widget_set_expand` |
| `uiWidgetSizeRequest(w,mw,mh)` | Minimum size | `slui_widget_set_size_request` |
| `uiWidgetSensitive(w,on)` | Enable / grey out | `slui_widget_set_sensitive` |
| `uiWidgetVisible(w,on)` | Show / hide | `slui_widget_set_visible` |
| `uiWidgetSetText(w,text)` / `uiWidgetGetText(w)` | Text accessor | `slui_widget_set/get_text` |
| `uiWidgetSetToggle(w,on)` / `uiWidgetGetToggle(w)` | Toggle state | `slui_widget_set/get_toggle` |
| `uiWidgetSetValue(w,v)` / `uiWidgetGetValue(w)` | Slider value | `slui_widget_set/get_value` |
| `uiWidgetSuggested(w,on)` / `uiWidgetDestructive(w,on)` | Accents | `slui_widget_set_suggested`/`_destructive` |
| `uiWidgetTakeActivated(w)` → int | 1 once per activation (latch cleared on read) | activate callback latch |
| `uiThemePreset(id)` → int | A theme filled from a preset; VM-local handle | `slui_theme_preset` |
| `uiThemeSet*(theme,color)` | Set a role colour (0xRRGGBBAA) | `SLUITheme` fields |
| `uiThemeSetRadius/ControlHeight(theme,px)` | Metrics | `SLUITheme` fields |
| `uiThemeSetFont(theme,family,size)` | Typography | `SLUITheme` fields |
| `uiThemeDestroy(theme)` | Release a theme handle | — |

> The native toolkit uses C **callbacks** for activation; from SLeeLa the VM
> records each activation in a per-widget latch that `uiWidgetTakeActivated`
> reads and clears, so a SLeeLa program observes a click/flip/edit exactly once
> per `pump()` turn without needing function pointers.

## The classes

### Application and window

| Class | Role |
|---|---|
| `SLUserInterface` | The capstone facade: create the application, report the live backend, open windows, run/pump/quit the event loop. |
| `SLWindow` | A top-level native window: show/hide, title, its root container, live theme swap, redraw. |

### Widgets

| Class | Role |
|---|---|
| `SLWidget` | The base for every control: margins, alignment, expansion, size request, sensitivity, visibility, text, suggested/destructive accents. |
| `SLBox` | A vertical/horizontal box container (GTK/CSS box layout). |
| `SLHeaderBar` | A title-bar strip (chrome fill + bottom hairline). |
| `SLLabel` | A non-interactive text label. |
| `SLButton` | A push button, with suggested (accent) and destructive variants. |
| `SLToggle` | A toggle switch with a trailing label. |
| `SLEntry` | A single-line UTF-8 text field with a caret. |
| `SLSlider` | A horizontal slider over a `[min,max]` range. |
| `SLSeparator` | A 1px hairline rule. |
| `SLSpacer` | A flexible expanding gap. |
| `SLCheckBox` | A labelled check box with an accent tick. |
| `SLRadioButton` | A grouped, mutually-exclusive radio button. |
| `SLComboBox` | A drop-down selection control. |
| `SLSpinButton` | A numeric value with − / + steppers over `[min,max]`. |
| `SLProgressBar` | A determinate progress bar over `[0,1]`. |
| `SLLevelBar` | A segmented level indicator over `[0,1]`. |
| `SLSpinner` | An indeterminate activity spinner. |
| `SLScrollBar` | A scroll bar with a draggable thumb. |
| `SLFrame` | A titled, bordered group container. |
| `SLCard` | A raised rounded surface panel. |
| `SLGrid` | A fixed row/column grid container. |
| `SLStatusBar` | A footer status strip. |
| `SLImage` | An image/icon tile with a centred glyph. |
| `SLAvatar` | A round accent avatar with an initial. |
| `SLBadge` | A small accent count/status pill. |
| `SLChip` | A rounded tag/filter pill. |
| `SLHeading` | A large title label. |
| `SLTooltip` | A small tooltip bubble. |
| `SLLinkButton` | A hyperlink-style accent button. |
| `SLSearchEntry` | An entry with a leading search glyph. |
| `SLPasswordEntry` | An entry that masks its characters. |
| `SLInfoBar` | An inline info/warning/error notice bar. |

> The full catalogue — every widget with its C ABI constructor, SLeeLa class,
> description, and focus behaviour — is tabulated in
> [`user-interface/WIDGETS.DESCRIPTION.md`](../../user-interface/WIDGETS.DESCRIPTION.md).

### Theme

| Class | Role |
|---|---|
| `SLTheme` | A palette handle. Start from the **Slick Black** default (`slickBlack()`) or `GRAPHITE`, then override any role colour, radius, control height, or font. |
| `SLColor` | A straight-alpha `0xRRGGBBAA` colour value helper (`rgb`/`rgba` + channel accessors). |

### Example

| Class | Role |
|---|---|
| `SLGalleryDemo` | A runnable worked example that builds a full Slick Black window with every control — the SLeeLa-source twin of `user-interface/examples/gallery.c`. |

## Minimal usage

```sleela
SLUserInterface ui = new SLUserInterface();
ui.configure("com.example.App");
if (ui.open()) {                               // false if no display (X11)
  SLTheme theme = new SLTheme();
  theme.slickBlack();                          // the default look

  SLWindow win = ui.openWindow("Hello", 480, 320, theme.handle());
  SLWidget root = win.rootWidget();

  SLBox col = new SLBox();
  col.vertical(root, 12);
  col.setMarginAll(20);

  SLLabel label = new SLLabel();
  label.createIn(col, "Welcome to SleelaUI");

  SLButton ok = new SLButton();
  ok.createIn(col, "Continue");
  ok.setSuggested(true);                       // the one accent action

  win.show();
  ui.run();                                    // run the toolkit event loop
}
```

A custom accent on top of Slick Black:

```sleela
SLTheme t = new SLTheme();
t.slickBlack();
SLColor green = new SLColor();
green.rgb(0x3d, 0xd6, 0x8c);
t.setAccentColor(green);
t.setRadius(12);
```

## Relationship to the rest of SLeeLa

- `lib/user-interface` is the SLeeLa-source **surface** of the native toolkit
  under [`/user-interface`](../../user-interface/); the C ABI there
  (`include/sleela_ui.h`) is what the `ui*` built-ins call.
- It parallels [`lib/os`](../os/OS.md): where `lib/os` drives the real host OS
  through the `os*` bridge, `lib/user-interface` drives the real host window
  system through the `ui*` bridge. Both expose one portable surface with native
  backends underneath and report which backend is live.
- `lib/gui` remains the general module facade; `lib/user-interface` is the
  concrete, class-rich toolkit binding.

## Note on fidelity

Every method here resolves to a genuine SleelaUI C ABI call, which raises a real
native window and paints it with the toolkit's own rasterizer on Windows, macOS,
and Linux/Unix. The look is defined by the portable toolkit core, not by any
native theme, so a SLeeLa program's interface is identical on all three
platforms — the project's stated UI quality bar (see
[`user-interface/UI-PRINCIPLES.md`](../../user-interface/UI-PRINCIPLES.md)).
