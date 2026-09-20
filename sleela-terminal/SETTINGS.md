# SleelaTerminal™ Settings

SleelaTerminal uses a compact three-horizontal-line menu in the title bar for presentation and orientation settings. The menu is intentionally small and keeps the terminal itself visually central.

## Presentation

The settings menu provides simple choices for:

- terminal font family and size;
- terminal foreground/font color; and
- a consistent dark terminal background.

The existing dark-purple title bar and three-dimensional purple footer remain the visual frame for the application.

## Language orientation

Input and output language are represented as separate configuration concepts so that a future implementation can distinguish the language used to enter commands/data from the language used to present information. The current menu provides a shared language selection for both and records the choice locally.

The current choices are System, English, Spanish, French, German, Korean, Chinese, and Japanese. Selecting a language does not by itself translate shell commands or external data; it establishes an explicit orientation setting for future language services.

## Parametric Ring of Trust / Orientation

The settings menu includes a small declarative model for possible information channels and linked parameters:

- **National / channel feeds** — declared feed or channel sources;
- **Persons of interest** — declared person-oriented information sources;
- **Parametric documents / resumes** — declared document or résumé sources;
- **National identifiers** — declared identifier-related sources.

These are deliberately modeled as **configuration declarations**, not as an identity system. A checked item means that the user has enabled that category in the local configuration; it does not mean that data has been retrieved, that a person has been identified, that a source is authoritative, or that any legal status has been established.

The ring-of-trust concept is therefore an orientation/model layer: it describes declared relationships among a terminal, a source category, and a parameter. It does not independently establish trust, truth, authority, identity, provenance, or legal validity.

## Configuration

The runtime configuration is intentionally human-readable:

```text
~/.config/sleela/sleela-terminal.conf
```

An example file is included as `sleela-terminal.conf.example`.

The current schema is:

```text
font=Monospace 11
foreground=#FFFFFF
input_language=system
output_language=system
national_feeds=false
persons_of_interest=false
parametric_documents=false
national_identifiers=false
```

Settings are loaded at startup and written when a menu choice changes. The application does not silently add external endpoints or credentials to this file.

## UI placement

The primary settings control lives in the title bar because presentation settings and orientation declarations are application-level concerns. Terminal text-selection operations remain in the terminal's right-click menu. The terminal remains the primary centered workspace, while contextual controls are exposed through GTK popovers rather than permanent panels.

Future additions such as image-backed footer content, moving text, scrolling information, or explicitly configured feed adapters should extend this same model rather than introduce a second settings system.
