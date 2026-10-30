<img align="right" src="https://github.com/mearvk/SLeeLa/blob/master/images/debian/debian-logo.png" width="75" height="75" alt="SLeeLa">

<img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela/sleela-logo-004.jpg" alt="SLeeLa">






# SLeeLa XML Examples

This directory contains clear, runnable examples for the SLeeLa BODI XML project system. The examples are also evidence artifacts: each XML project has a corresponding expected BODI witness output where the result is deterministic and does not require external network access.

## Native SleelaUI GUI learning examples

The full SLeeLa-source GUI catalogue is in [gui/](gui/README.md). It covers
native window lifecycle, the five current theme presets plus legacy/custom
palette examples, component families, common widget properties, tabs, forms,
event polling, backend reporting, and the separate `lib/ui` SDK object model.
These examples target the native wrappers in `lib/user-interface/` when they
open real desktop windows.

Start with [CLASSIC Hello Window](gui/CLASSIC-hello-window.sleela), then explore
the [full widget gallery](gui/USED-widget-gallery.sleela) and
[theme presets](gui/USED-theme-presets.sleela).

## GUI connectivity examples

The interactive native GUI examples in [../example/](../example/README.md) demonstrate the shared Wi-Fi control. Each of the five GUI versions — DUAL, GUAL, MUAL, THUAL, and MILITARY — uses an icon-backed **Wi-Fi** button and a dismissible connectivity panel with actions for scanning, connecting, disconnecting, network settings, and connection details.

- [DUAL media studio](../example/dual/gui-dual-media-studio.sleela)
- [GUAL settings console](../example/gual/gui-gual-settings-console.sleela)
- [MUAL text editor](../example/mual/gui-mual-text-editor.sleela)
- [THUAL effects showcase](../example/thual/gui-thual-effects-showcase.sleela)
- [MILITARY command console](../example/military/gui-military-command-console.sleela)

These are GUI examples, not BODI XML projects, so they remain in /example rather than being represented as XML entries here. The menu reports when the native Wi-Fi host bridge is unavailable; it does not fabricate scan results or claim to change the real network connection. The JPEG icon uses the named Wifi vector fallback if the asset cannot be loaded.

## Example families

| Example | Purpose | Expected evidence |
|---|---|---|
| 01 | Mathematics | BODI witness from the math dispatcher |
| 02 | Physics | BODI witness from the physics dispatcher |
| 03 | Chemistry | BODI witness from the chemistry dispatcher |
| 04 | Mixed science | Witnesses from multiple science disciplines |\n| 11 | Data analytics | `11-data-analytics.expected.txt` |
| 05 | SMTP email | Explicit network submission example |
| 06 | SMTP dry run | Safe SMTP configuration validation |
| 07 | HTTP-style POST | `07-post.expected.txt` |
| 08 | Listener declaration | `08-listener.expected.txt` |
| 09 | Router declaration | `09-router.expected.txt` |

## Running

Build the BODI runner:

    cd ../api/bodi
    make

Run the examples:

    ../api/bodi/bodi-xml-runner 01-math.xml
    ../api/bodi/bodi-xml-runner 02-physics.xml
    ../api/bodi/bodi-xml-runner 03-chemistry.xml
    ../api/bodi/bodi-xml-runner 04-mixed-science.xml
    ../api/bodi/bodi-xml-runner 06-email-dry-run.xml
    ../api/bodi/bodi-xml-runner 07-post.xml
    ../api/bodi/bodi-xml-runner 08-listener.xml
    ../api/bodi/bodi-xml-runner 09-router.xml\n    ../api/bodi/bodi-xml-runner 11-data-analytics.xml

SMTP transmission is intentionally explicit:

    ../api/bodi/bodi-xml-runner 05-email-smtp.xml --send-email

Credentials are referenced through environment variables rather than stored in XML.

## XML vocabulary

Every project starts with `<bodi version="1">` and a `<project>` declaration.

The current project kinds are:

- `science` — executable math, physics, chemistry, and data-analytics operations.
- `email` — SMTP configuration and message submission.
- `post` — HTTP-style POST representation.
- `listener` — listener bind/protocol/route declaration.
- `router` — deterministic method/path-to-target route declarations.

The native C vocabulary is under:

- `api/posting/`
- `api/listener/`
- `api/router/`

XML is declarative and allow-listed. It cannot invoke arbitrary shell commands or arbitrary native functions. The post, listener, and router examples validate and witness their declarations without opening sockets or transmitting network traffic.

## 10 — Apache/Tomcat Web Server Deployment

`10-webserver-deployment.md` shows how a web-aware application can hand its deployment artifact and health endpoint to the cross-platform `api/webserver/` module.

---

# SLeeLa
<p align="center"><img src="images/sleela/sleela-wordmark.svg" alt="SLeeLa — The Now. The Ingredient." width="560"></p>
<p align="center"><img src="https://github.com/mearvk/SLeeLa/blob/master/images/sleela-programming-language-002.jpg" alt="SLeeLa Marketing Logo" width="560"></p>
