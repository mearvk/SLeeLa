# Video Cards (GPUs) — Specification (video-cards)

A graphics card drives the display and performs parallel graphics/compute work.
`lib/os/video-cards` models the finite set of GPU variants by vendor,
architecture, board interface, video memory, display outputs, power, and APIs.

## GPU vendors and architectures

| Vendor | Brand | Recent architecture | Representative models |
|---|---|---|---|
| NVIDIA | GeForce / RTX | Ada Lovelace | RTX 4090 / 4080 / 4070 |
| AMD | Radeon RX | RDNA 3 | RX 7900 XTX / 7800 XT |
| Intel | Arc | Xe-HPG | Arc A770 / A750 |

Discrete GPU silicon comes only from these three vendors; **board partners**
(ASUS, MSI, Gigabyte, Sapphire, PowerColor, Zotac, XFX) ship the same GPUs with
different coolers/clocks. `SLVideoCardCatalog` exposes vendors and board partners
by name.

## Board interface

Modern cards use **PCIe x16** (gen 3/4/5). The lane count and generation set the
host bandwidth ceiling; `SLVideoCard.setPcie(gen, lanes)` records it.

## Video memory

| Type | Notes |
|---|---|
| GDDR6 | mainstream |
| GDDR6X | higher-bandwidth NVIDIA variant |
| GDDR7 | newest generation |
| HBM | stacked high-bandwidth memory (data-center / workstation) |

VRAM size (e.g. 24 GB on an RTX 4090) is carried in MB by `vramMegabytes()`.

## Display outputs

Cards expose a mix of **HDMI** and **DisplayPort** connectors; `outputsTotal()`
is their sum. The number and version set the maximum resolution/refresh.

## Graphics / compute APIs

Cards advertise **Vulkan**, **OpenGL**, **Direct3D 12** (Windows), and vendor
compute stacks (CUDA, ROCm, oneAPI). `supportsVulkan()` / `supportsDirect3D()`
reflect the portable graphics APIs the display stack (`SLDesktopSpec`) can use.

## Relationship to the OS model

The video card is the display adapter the desktop/session layer renders through.
A headless/server machine composes no video card; a desktop machine composes one
and the `SLDesktopSpec` display server (Wayland / DWM / WindowServer) drives it.
