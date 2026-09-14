# modeling/ — Phraign™ City 3D model data

This subfolder holds **city model data**: serialized `.city` files in the
`PHRAIGN-CITY` text format (see [`../MODEL_FORMAT.md`](../MODEL_FORMAT.md)).

A model is generated per user from a seed, so a given user always reproduces the
same city. Models are plain, line-oriented text — they diff cleanly and are
meant to be saved here, committed to GitHub, or uploaded to a public server.

## Contents

| File | Grid | Blocks | Notes |
|------|------|--------|-------|
| `demo.city` | 64×64 | 4096 (~4000) | Default sprawling city, `user=demo` |
| `smalltown.city` | 32×32 | 1024 | Smaller example, blue theme seed |

## Working with models

```sh
# from bash/model-3D/
make                                   # build ./build/city3d

# Generate a new model into modeling/
./build/city3d generate --config city.config --out modeling/mycity.city

# Render a saved model to a preview image (per-pixel Phraign frame -> PPM)
./build/city3d render --model modeling/mycity.city --config city.config \
    --ppm build/mycity.ppm

# Show the save plan for the configured target (github|server|local)
./build/city3d save --model modeling/mycity.city --config city.config
```

New users typically drop their own `<name>.city` here. Because the format is
deterministic from `(user, seed, grid)`, a model file plus the config is enough
to reproduce and re-render the exact city on any machine.
