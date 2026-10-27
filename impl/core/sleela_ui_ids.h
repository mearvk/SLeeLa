/* ===========================================================================
 * sleela_ui_ids.h -- stable integer ids for the ui* SLVM->SleelaUI bridge.
 *
 * The SLeeLa /lib/user-interface and /lib/gui classes call ui* built-ins
 * (uiAppCreate, uiWindowCreate, uiButton, uiWidgetTakeActivated, ...). The
 * compiler lowers each ui* call to a single OP_UI_CALL opcode whose operand is
 * one of these ids; the VM bridge (sleela_ui_vm.c) routes the id to the genuine
 * SleelaUI C ABI (user-interface/include/sleela_ui.h), which drives the real
 * host window system (X11/Cocoa/Win32). Keeping the id table in one header
 * shared by the compiler and the VM guarantees they stay in lock-step.
 *
 * Each entry's comment records the SLeeLa-facing arity and argument shape so
 * the compiler can arity-check and the VM knows how many values to pop.
 * (i = int handle/number, s = string, d = double)
 * =========================================================================== */
#ifndef SLEELA_UI_IDS_H
#define SLEELA_UI_IDS_H

typedef enum {
    /* --- application + process (SLUIApp) --- */
    SLUI_FN_APP_CREATE = 1,   /* uiAppCreate(appId:s) -> app handle            */
    SLUI_FN_APP_RUN,          /* uiAppRun(app:i) -> exit code                  */
    SLUI_FN_APP_PUMP,         /* uiAppPump(app:i, block:i) -> dispatched count */
    SLUI_FN_APP_QUIT,         /* uiAppQuit(app:i, code:i) -> null              */
    SLUI_FN_APP_DESTROY,      /* uiAppDestroy(app:i) -> null                   */
    SLUI_FN_BACKEND,          /* uiBackend() -> backend id                     */
    SLUI_FN_BACKEND_NAME,     /* uiBackendName(id:i) -> name string            */
    SLUI_FN_VERSION,          /* uiVersion() -> version string                 */

    /* --- window (SLUIWindow) --- */
    SLUI_FN_WINDOW_CREATE,    /* uiWindowCreate(app:i,title:s,w:i,h:i,theme:i) -> window */
    SLUI_FN_WINDOW_SHOW,      /* uiWindowShow(window:i) -> null                 */
    SLUI_FN_WINDOW_HIDE,      /* uiWindowHide(window:i) -> null                 */
    SLUI_FN_WINDOW_ROOT,      /* uiWindowRoot(window:i) -> root widget          */
    SLUI_FN_WINDOW_SET_TITLE, /* uiWindowSetTitle(window:i, title:s) -> null    */
    SLUI_FN_WINDOW_SET_THEME, /* uiWindowSetTheme(window:i, theme:i) -> null    */
    SLUI_FN_WINDOW_REDRAW,    /* uiWindowRedraw(window:i) -> null               */
    SLUI_FN_WINDOW_SET_BUFFERING, /* uiWindowSetBuffering(window:i, mode:i) -> null (0 single/1 double/2 triple) */
    SLUI_FN_WINDOW_BUFFERING, /* uiWindowBuffering(window:i) -> mode int        */

    /* --- containers / structure --- */
    SLUI_FN_BOX,              /* uiBox(parent:i, orient:i, spacing:i) -> widget */
    SLUI_FN_GRID,             /* uiGrid(parent:i, cols:i, spacing:i) -> widget  */
    SLUI_FN_FRAME,            /* uiFrame(parent:i, title:s) -> widget           */
    SLUI_FN_CARD,             /* uiCard(parent:i) -> widget                     */
    SLUI_FN_HEADER_BAR,       /* uiHeaderBar(parent:i) -> widget                */
    SLUI_FN_STATUS_BAR,       /* uiStatusBar(parent:i) -> widget                */
    SLUI_FN_SEPARATOR,        /* uiSeparator(parent:i, orient:i) -> widget      */
    SLUI_FN_SPACER,           /* uiSpacer(parent:i) -> widget                   */
    SLUI_FN_SCROLLED_VIEW,    /* uiScrolledView(parent:i, polH:i, polV:i) -> widget; 1st child = content */
    SLUI_FN_SCROLLED_SCROLL_TO, /* uiScrolledScrollTo(view:i, x:i, y:i) -> 0     */
    SLUI_FN_SCROLLED_SCROLL_BY, /* uiScrolledScrollBy(view:i, dx:i, dy:i) -> 0   */
    SLUI_FN_SCROLLED_SCROLL_X,  /* uiScrolledScrollX(view:i) -> offset px        */
    SLUI_FN_SCROLLED_SCROLL_Y,  /* uiScrolledScrollY(view:i) -> offset px        */

    /* --- buttons / actions --- */
    SLUI_FN_BUTTON,           /* uiButton(parent:i, text:s) -> widget           */
    SLUI_FN_LINK_BUTTON,      /* uiLinkButton(parent:i, text:s) -> widget       */
    SLUI_FN_TOGGLE,           /* uiToggle(parent:i, text:s, on:i) -> widget     */

    /* --- selection / boolean --- */
    SLUI_FN_CHECK_BOX,        /* uiCheckBox(parent:i, text:s, on:i) -> widget   */
    SLUI_FN_RADIO_BUTTON,     /* uiRadioButton(parent:i, text:s, group:i, on:i) -> widget */
    SLUI_FN_COMBO_BOX,        /* uiComboBox(parent:i) -> widget                 */
    SLUI_FN_COMBO_BOX_ADD,    /* uiComboBoxAdd(combo:i, option:s) -> null       */
    SLUI_FN_SPIN_BUTTON,      /* uiSpinButton(parent:i, min:d, max:d, step:d, val:d) -> widget */

    /* --- text input --- */
    SLUI_FN_ENTRY,            /* uiEntry(parent:i, placeholder:s) -> widget     */
    SLUI_FN_SEARCH_ENTRY,     /* uiSearchEntry(parent:i, placeholder:s) -> widget */
    SLUI_FN_PASSWORD_ENTRY,   /* uiPasswordEntry(parent:i, placeholder:s) -> widget */

    /* --- ranges / indicators --- */
    SLUI_FN_SLIDER,           /* uiSlider(parent:i, min:d, max:d, val:d) -> widget */
    SLUI_FN_SCROLL_BAR,       /* uiScrollBar(parent:i, orient:i, value:d, page:d) -> widget */
    SLUI_FN_PROGRESS_BAR,     /* uiProgressBar(parent:i, fraction:d) -> widget  */
    SLUI_FN_LEVEL_BAR,        /* uiLevelBar(parent:i, fraction:d) -> widget     */
    SLUI_FN_SPINNER,          /* uiSpinner(parent:i) -> widget                  */

    /* --- text / display --- */
    SLUI_FN_LABEL,            /* uiLabel(parent:i, text:s) -> widget            */
    SLUI_FN_HEADING,          /* uiHeading(parent:i, text:s, size:i) -> widget  */
    SLUI_FN_IMAGE,            /* uiImage(parent:i, glyph:s, w:i, h:i) -> widget */
    SLUI_FN_AVATAR,           /* uiAvatar(parent:i, initial:s, diameter:i) -> widget */

    /* --- tags / feedback --- */
    SLUI_FN_BADGE,            /* uiBadge(parent:i, text:s) -> widget            */
    SLUI_FN_CHIP,             /* uiChip(parent:i, text:s) -> widget             */
    SLUI_FN_INFO_BAR,         /* uiInfoBar(parent:i, text:s, severity:i) -> widget */
    SLUI_FN_TOOLTIP,          /* uiTooltip(parent:i, text:s) -> widget          */

    /* --- motion / drawing --- */
    SLUI_FN_THROBBER,         /* uiThrobber(parent:i, width:i) -> widget        */
    SLUI_FN_THROBBER_SET_WIDTH,     /* uiThrobberSetWidth(w:i, px:i) -> null    */
    SLUI_FN_THROBBER_SET_INTENSITY, /* uiThrobberSetIntensity(w:i, v:d) -> null */
    SLUI_FN_THROBBER_SET_HUE,       /* uiThrobberSetHue(w:i, deg:d) -> null     */

    /* --- widget configuration (setters) --- */
    SLUI_FN_WIDGET_SET_MARGIN,   /* uiWidgetMargin(w:i, top:i,right:i,bottom:i,left:i) -> null */
    SLUI_FN_WIDGET_SET_ALIGN,    /* uiWidgetAlign(w:i, h:i, v:i) -> null        */
    SLUI_FN_WIDGET_SET_EXPAND,   /* uiWidgetExpand(w:i, h:i, v:i) -> null       */
    SLUI_FN_WIDGET_SET_SIZE_REQ, /* uiWidgetSizeRequest(w:i, minW:i, minH:i) -> null */
    SLUI_FN_WIDGET_SET_SENSITIVE,/* uiWidgetSensitive(w:i, on:i) -> null        */
    SLUI_FN_WIDGET_SET_VISIBLE,  /* uiWidgetVisible(w:i, on:i) -> null          */
    SLUI_FN_WIDGET_SET_SUGGESTED,/* uiWidgetSuggested(w:i, on:i) -> null        */
    SLUI_FN_WIDGET_SET_DESTRUCT, /* uiWidgetDestructive(w:i, on:i) -> null      */
    SLUI_FN_WIDGET_SET_TEXT,     /* uiWidgetSetText(w:i, text:s) -> null        */
    SLUI_FN_WIDGET_SET_TOGGLE,   /* uiWidgetSetToggle(w:i, on:i) -> null        */
    SLUI_FN_WIDGET_SET_VALUE,    /* uiWidgetSetValue(w:i, val:d) -> null        */

    /* --- widget readback (getters) --- */
    SLUI_FN_WIDGET_GET_TEXT,     /* uiWidgetGetText(w:i) -> string              */
    SLUI_FN_WIDGET_GET_TOGGLE,   /* uiWidgetGetToggle(w:i) -> int               */
    SLUI_FN_WIDGET_GET_VALUE,    /* uiWidgetGetValue(w:i) -> double             */
    SLUI_FN_WIDGET_TAKE_ACTIVATED, /* uiWidgetTakeActivated(w:i) -> int         */

    /* --- theme (SLUITheme) --- */
    SLUI_FN_THEME_PRESET,        /* uiThemePreset(presetId:i) -> theme handle   */
    SLUI_FN_THEME_DESTROY,       /* uiThemeDestroy(theme:i) -> null             */
    SLUI_FN_THEME_LOAD_CONFIG,   /* uiThemeLoadConfig(theme:i, path:s) -> status int */
    SLUI_FN_THEME_SET_BASE,      /* uiThemeSetBaseColor(theme:i, color:i) -> null */
    SLUI_FN_THEME_SET_BG,        /* uiThemeSetBg(theme:i, color:i) -> null      */
    SLUI_FN_THEME_SET_SURFACE,   /* uiThemeSetSurface(theme:i, color:i) -> null */
    SLUI_FN_THEME_SET_SURFACE_HI,/* uiThemeSetSurfaceHi(theme:i, color:i) -> null */
    SLUI_FN_THEME_SET_CHROME,    /* uiThemeSetChrome(theme:i, color:i) -> null  */
    SLUI_FN_THEME_SET_BORDER,    /* uiThemeSetBorder(theme:i, color:i) -> null  */
    SLUI_FN_THEME_SET_FG,        /* uiThemeSetFg(theme:i, color:i) -> null      */
    SLUI_FN_THEME_SET_FG_DIM,    /* uiThemeSetFgDim(theme:i, color:i) -> null   */
    SLUI_FN_THEME_SET_ACCENT,    /* uiThemeSetAccent(theme:i, color:i) -> null  */
    SLUI_FN_THEME_SET_ACCENT_FG, /* uiThemeSetAccentFg(theme:i, color:i) -> null */
    SLUI_FN_THEME_SET_DANGER,    /* uiThemeSetDanger(theme:i, color:i) -> null  */
    SLUI_FN_THEME_SET_RADIUS,    /* uiThemeSetRadius(theme:i, px:i) -> null     */
    SLUI_FN_THEME_SET_CONTROL_H, /* uiThemeSetControlHeight(theme:i, px:i) -> null */
    SLUI_FN_THEME_SET_FONT,      /* uiThemeSetFont(theme:i, family:s, sizePt:i) -> null */

    /* --- screen / display geometry --- */
    SLUI_FN_SCREEN_WIDTH,        /* uiScreenWidth(app:i)  -> display width px, or 0  */
    SLUI_FN_SCREEN_HEIGHT,       /* uiScreenHeight(app:i) -> display height px, or 0 */

    SLUI_FN_WIDGET_SET_ICON,    /* uiWidgetSetIcon(w:i, assetPath:s, symbol:s) */
    SLUI_FN_POPUP_CREATE,       /* uiPopupCreate(parent:i) -> hidden popup panel */
    SLUI_FN_POPUP_SET_OPEN,     /* uiPopupSetOpen(popup:i, open:i) -> null */
    SLUI_FN_POPUP_IS_OPEN,      /* uiPopupIsOpen(popup:i) -> int */
    SLUI_FN_WIFI_OPEN_SETTINGS, /* uiWifiOpenSettings() -> status */
    SLUI_FN_WIFI_SCAN,          /* uiWifiScan() -> status */
    SLUI_FN_WIFI_NETWORK_COUNT, /* uiWifiNetworkCount() -> int */
    SLUI_FN_WIFI_NETWORK_SSID,  /* uiWifiNetworkSsid(index:i) -> string */
    SLUI_FN_WIFI_NETWORK_BSSID, /* uiWifiNetworkBssid(index:i) -> string */
    SLUI_FN_WIFI_NETWORK_SIGNAL,/* uiWifiNetworkSignal(index:i) -> int */
    SLUI_FN_WIFI_NETWORK_SECURITY,/* uiWifiNetworkSecurity(index:i) -> string */
    SLUI_FN_WIFI_CONNECT,       /* uiWifiConnect(index:i, profile:s) -> status */
    SLUI_FN_WIFI_DISCONNECT,    /* uiWifiDisconnect() -> status */
    SLUI_FN_WIFI_STATUS_NAME,   /* uiWifiStatusName(status:i) -> string */

    /* --- Ballum(TM): pixel-level display control (ballum/ + lib/ballum) --- */
    SLUI_FN_BALLUM_SUPPORTED,        /* uiBallumSupported() -> 1 real backend, 0 fallback */
    SLUI_FN_BALLUM_BACKEND_NAME,     /* uiBallumBackendName() -> string */
    SLUI_FN_BALLUM_ENUMERATE,        /* uiBallumEnumerate() -> status */
    SLUI_FN_BALLUM_DISPLAY_COUNT,    /* uiBallumDisplayCount() -> int */
    SLUI_FN_BALLUM_DISPLAY_ID,       /* uiBallumDisplayId(index:i) -> id, or -1 */
    SLUI_FN_BALLUM_DISPLAY_NAME,     /* uiBallumDisplayName(index:i) -> string */
    SLUI_FN_BALLUM_DISPLAY_WIDTH,    /* uiBallumDisplayWidth(index:i) -> px */
    SLUI_FN_BALLUM_DISPLAY_HEIGHT,   /* uiBallumDisplayHeight(index:i) -> px */
    SLUI_FN_BALLUM_DISPLAY_PRIMARY,  /* uiBallumDisplayPrimary(index:i) -> 1/0/-1 */
    SLUI_FN_BALLUM_DISPLAY_CAPS,     /* uiBallumDisplayCaps(index:i) -> capability bitmask */
    SLUI_FN_BALLUM_SET_PIXEL,        /* uiBallumSetPixel(id:i,x:i,y:i,r:i,g:i,b:i,a:i) -> status */
    SLUI_FN_BALLUM_GET_PIXEL,        /* uiBallumGetPixel(id:i,x:i,y:i) -> packed 0xAARRGGBB, or -1 */
    SLUI_FN_BALLUM_SET_BRIGHTNESS,   /* uiBallumSetBrightness(id:i, percent:i) -> status */
    SLUI_FN_BALLUM_SET_WARMTH,       /* uiBallumSetWarmth(id:i, kelvin:i) -> status */
    SLUI_FN_BALLUM_SET_GAMMA,        /* uiBallumSetGamma(id:i, gammaScaled:i) -> status */
    SLUI_FN_BALLUM_STATUS_NAME,      /* uiBallumStatusName(status:i) -> string */
    /* Ballum pointer input (mouse/touch): click, move, drag. */
    SLUI_FN_BALLUM_HAS_INPUT,        /* uiBallumHasInput() -> 1/0 */
    SLUI_FN_BALLUM_POLL,             /* uiBallumPoll() -> event type (0 none,1 down,2 move,3 up) */
    SLUI_FN_BALLUM_EVENT_X,          /* uiBallumEventX() -> px */
    SLUI_FN_BALLUM_EVENT_Y,          /* uiBallumEventY() -> px */
    SLUI_FN_BALLUM_EVENT_BUTTON,     /* uiBallumEventButton() -> 0..3 */
    SLUI_FN_BALLUM_EVENT_SOURCE,     /* uiBallumEventSource() -> 0 mouse,1 touch */
    SLUI_FN_BALLUM_EVENT_CONTACT,    /* uiBallumEventContact() -> touch contact id */
    SLUI_FN_BALLUM_EVENT_MODIFIERS,  /* uiBallumEventModifiers() -> bitmask */
    SLUI_FN_BALLUM_EVENT_TYPE_NAME,  /* uiBallumEventTypeName(type:i) -> string */
    SLUI_FN_BALLUM_HAS_KEYBOARD,     /* uiBallumHasKeyboard() -> 1/0 */
    SLUI_FN_BALLUM_POLL_KEY,         /* uiBallumPollKey() -> key code (0 none, 27 ESC, 13 ENTER, 32 SPACE) */
    SLUI_FN_BALLUM_LOGO_READY,       /* uiBallumLogoReady() -> 1 logo decoded + loaded, 0 not */
    SLUI_FN_BALLUM_LOGO_BLEND,       /* uiBallumLogoBlend(id:i,pct:i,w:i,h:i,r:i,g:i,b:i) -> status */
    SLUI_FN_BALLUM_LOGO_LOAD,        /* uiBallumLogoLoad(path:s) -> status (decode a JPEG logo) */
    SLUI_FN_BALLUM_LOGO_SET_EFFECT,  /* uiBallumLogoSetEffect(fx:i) -> status */
    SLUI_FN_BALLUM_FX_COUNT,         /* uiBallumFxCount() -> number of effects */
    SLUI_FN_BALLUM_FX_NAME,          /* uiBallumFxName(fx:i) -> string name */
    SLUI_FN_BALLUM_FX_BY_NAME,       /* uiBallumFxByName(name:s) -> effect id */

    /* --- pointer interaction polling (tab/button state) --- */
    SLUI_FN_WIDGET_IS_HOVERED,
    SLUI_FN_WIDGET_TAKE_PRESSED,
    SLUI_FN_WIDGET_TAKE_RELEASED,
    SLUI_FN_WIDGET_TAKE_HOVER_ENTERED,
    SLUI_FN_WIDGET_TAKE_HOVER_LEFT,
    /* Window size limits (pixel dimensions; max 0 means unbounded). */
    SLUI_FN_WINDOW_SET_MIN_SIZE,       /* uiWindowSetMinSize(window:i,w:i,h:i) -> status */
    SLUI_FN_WINDOW_SET_MAX_SIZE,       /* uiWindowSetMaxSize(window:i,w:i,h:i) -> status */
    SLUI_FN_WINDOW_SET_SIZE_LIMITS,    /* uiWindowSetSizeLimits(window:i,minW:i,minH:i,maxW:i,maxH:i) -> status */
    SLUI_FN_WINDOW_MIN_WIDTH,          /* uiWindowMinWidth(window:i) -> px or -1 */
    SLUI_FN_WINDOW_MIN_HEIGHT,         /* uiWindowMinHeight(window:i) -> px or -1 */
    SLUI_FN_WINDOW_MAX_WIDTH,          /* uiWindowMaxWidth(window:i) -> px/0 or -1 */
    SLUI_FN_WINDOW_MAX_HEIGHT,         /* uiWindowMaxHeight(window:i) -> px/0 or -1 */
    SLUI_FN__COUNT               /* KEEP LAST: one past the last id             */
} SleelaUiFnId;

#endif /* SLEELA_UI_IDS_H */
