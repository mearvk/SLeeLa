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

### Year — modernity

The city is a **model of finality** (quality of condition) driven by a Year.
A larger, more modern year makes the city newer: taller buildings, more floors
and windows, more glass, and better condition. Try a modern future number like
`2807` or `2407`.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `year` | int > 0 | `2807` | The city's Year (drives modernity) |
| `year_baseline` | int | `2000` | Year mapped to modernity `0` |
| `year_span` | float > 0 | `1000` | Years above the baseline that reach modernity `1` |

Modernity is `clamp((year - year_baseline) / year_span, 0..1)`. It scales the
building form and feeds finality (below). For example `year = 2807` gives
modernity `0.807`; `year = 2050` gives `0.05`.

### Building quality targets

Buildings vary around these targets; modernity scales them up as the city
becomes newer.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `avg_floors` | float ≥ 0 | `14` | Target mean number of floors |
| `floor_height` | float > 0 | `1.1` | World units per floor (height = floors × this) |
| `windows_per_floor` | float ≥ 0 | `6` | Target windows on a visible face, per floor |

Each building stores its own `floors`, `windows`, and `height`. Window detailing
is drawn on the two visible faces when `draw_windows` is on.

### Roads & bridges — real features

Roads are corridors laid every `road_spacing` blocks; bridges are spans laid
along those corridors. Proximity to roads and bridges is **computed** per
building (a breadth-first distance over the grid — not a config weight), and
that computed distance feeds finality through the weights below.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `road_spacing` | int ≥ 2 | `8` | Every Nth row/column is a road corridor |
| `bridge_count` | int ≥ 0 | `6` | Number of bridge spans placed on the road grid |

### Finality — quality of condition

Each building's **finality** is a score in `[0,1]`: how finished, modern, and
well-conditioned it is. It is a normalized weighted mix of modernity (Year),
floors, windows, and the computed proximity to the nearest road and bridge. The
city-wide finality (mean over buildings) is reported by `city3d` and stored in
the model. Higher finality renders brighter/cleaner; lower renders darker/duller.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `w_year` | float ≥ 0 | `1.0` | Weight of modernity (newer ⇒ higher finality) |
| `w_floors` | float ≥ 0 | `0.6` | Weight of floor count (more ⇒ more finished/dense) |
| `w_windows` | float ≥ 0 | `0.6` | Weight of windows (more ⇒ more modern/glassy) |
| `w_road_proximity` | float ≥ 0 | `0.8` | Weight of closeness to a road (better serviced) |
| `w_bridge_proximity` | float ≥ 0 | `0.5` | Weight of closeness to a bridge (better connected) |
| `proximity_falloff` | float > 0 | `6.0` | Distance (blocks) at which proximity benefit fades to 0 |

Only the relative sizes of the weights matter (they are normalized).

### Appearance — color theme

| Key | Values | Default | Meaning |
|-----|--------|---------|---------|
| `theme` | `green` \| `white` \| `blue` | `green` | Color palette for the render |

**Green is the default.** Each theme resolves to a palette (background, ground,
roof, lit wall, shaded wall) so buildings read with a lit top/side and a darker
shaded side. Face brightness is then modulated by each building's finality.

### Detailing toggles

| Key | Values | Default | Meaning |
|-----|--------|---------|---------|
| `draw_bridges` | bool | `true` | Draw bridge spans as raised decks |
| `draw_windows` | bool | `true` | Draw window bands on visible building faces |

Booleans accept `true`/`false`, `1`/`0`, `yes`/`no`, `on`/`off`.

### Viewpoint — seen from the top and slightly to the side

The camera is an **oblique projection**: the city is viewed from above and
angled slightly to one side. It is fully described by these scalars, so the
viewpoint is modifiable entirely from the config file.

| Key | Type | Default | Meaning |
|-----|------|---------|---------|
| `tilt_x` | float | `0.55` | Horizontal skew per unit of depth (side angle) |
| `tilt_y` | float | `0.42` | Vertical foreshortening per unit of depth (top-down amount) |
| `scale_x` | float | `9.0` | Pixels per block along X (east/west) |
| `scale_height` | float | `1.1` | Pixels per unit of building height |
| `block_pitch` | float | `9.0` | Pixels between block centers |
| `origin_x` | float | `150.0` | Screen X (px) where the city's near corner lands |
| `origin_y` | float | `380.0` | Screen Y (px) where the city's near corner lands |

The taller, modern default cities need a smaller `scale_height` and a lower
`origin_y` than a short city so the skyline fits the frame.

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

# A modern, high-finality city.
year          = 2807
avg_floors    = 16
windows_per_floor = 8
road_spacing  = 8
bridge_count  = 6
w_road_proximity = 0.8

theme        = blue
draw_bridges = true
draw_windows = true

tilt_x       = 0.55
tilt_y       = 0.42
scale_x      = 9.0
scale_height = 1.1
block_pitch  = 9.0
origin_x     = 150.0
origin_y     = 380.0

frame_width  = 1024
frame_height = 768

save_target   = github
save_location = mearvk/SLeeLa
```
