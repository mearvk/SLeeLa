# City 3D configuration

City 3D reads a simple text config file (`city.config` by default). The format
is `key = value`, one per line; `#` begins a comment; blank lines are ignored.
**Unknown keys are ignored** (forward-compatible) and **missing keys keep their
defaults**, so a partial config is always valid.

## Keys

### Model

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `grid_cols` | int > 0 | `64` | City width in blocks |
| `grid_rows` | int > 0 | `64` | City height in blocks (64×64 = 4096 ≈ 4000) |
| `user` | text | `anon` | Per-user identity |
| `seed` | uint64 | `0` | Generation seed; `0` ⇒ derive a stable seed from `user` |

Every user gets their own reproducible city: with `seed = 0`, the seed is
derived from the `user` name, so the same user always regenerates the same city.
Set an explicit non-zero `seed` to pin a specific layout.

### Appearance — color theme

| Key | Values | Default | Meaning |
|-----|--------|---------|---------|
| `theme` | `green` \| `white` \| `blue` | `green` | Color palette for the render |

**Green is the default.** Each theme resolves to a palette (background, ground,
roof, lit wall, shaded wall) so buildings read with a lit top/side and a darker
shaded side.

### Viewpoint — seen from the top and slightly to the side

The camera is an **oblique projection**: the city is viewed from above and
angled slightly to one side. It is fully described by these scalars, so the
viewpoint is modifiable entirely from the config file.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `tilt_x` | float | `0.55` | Horizontal skew per unit of depth (side angle) |
| `tilt_y` | float | `0.42` | Vertical foreshortening per unit of depth (top-down amount) |
| `scale_x` | float | `9.0` | Pixels per block along X (east/west) |
| `scale_height` | float | `3.0` | Pixels per unit of building height |
| `block_pitch` | float | `9.0` | Pixels between block centers |
| `origin_x` | float | `150.0` | Screen X (px) where the city's near corner lands |
| `origin_y` | float | `210.0` | Screen Y (px) where the city's near corner lands |

Tuning hints:

- **More top-down:** lower `tilt_x` and `tilt_y` toward `0`.
- **Stronger side angle:** raise `tilt_x`.
- **Taller skyline:** raise `scale_height`.
- **Reposition / recenter:** adjust `origin_x` / `origin_y`.

### Frame — the Phraign per-pixel render target

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `frame_width` | int > 0 | `1024` | Phraign frame width in pixels |
| `frame_height` | int > 0 | `768` | Phraign frame height in pixels |

### Persistence — where a saved model lives

| Key | Values | Default | Meaning |
|-----|--------|---------|---------|
| `save_target` | `github` \| `server` \| `local` | `local` | Destination kind |
| `save_location` | text | *(empty)* | `owner/repo` (github), upload URL (server), or path (local) |

`city3d save` prints the save plan for the configured target (for example, the
`git add/commit/push` steps for `github`, or an upload command for `server`).

## Example

```ini
grid_cols = 64
grid_rows = 64
user      = ada
seed      = 0

theme = blue

tilt_x       = 0.55
tilt_y       = 0.42
scale_x      = 9.0
scale_height = 3.0
block_pitch  = 9.0
origin_x     = 150.0
origin_y     = 210.0

frame_width  = 1024
frame_height = 768

save_target   = github
save_location = mearvk/SLeeLa
```
