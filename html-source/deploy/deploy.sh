#!/usr/bin/env bash
# One-command Linux deploy with Docker: fetch/refresh the SLeeLa checkout, build the image, start the site.
#   ./deploy/deploy.sh                      # http://localhost:8080
#   SLEELA_SRC=/srv/sleela SITE_PORT=80 ./deploy/deploy.sh
set -euo pipefail
cd "$(dirname "$0")/.."

SLEELA_SRC="${SLEELA_SRC:-/srv/sleela}"
SITE_PORT="${SITE_PORT:-8080}"
export SLEELA_SRC SITE_PORT

command -v docker >/dev/null || { echo "docker is required" >&2; exit 1; }

if [ -d "$SLEELA_SRC/.git" ]; then
  git -C "$SLEELA_SRC" pull --ff-only
elif [ -d "$SLEELA_SRC/tutorial" ]; then
  echo "using existing content at $SLEELA_SRC"
else
  mkdir -p "$(dirname "$SLEELA_SRC")"
  git clone --depth 1 https://github.com/mearvk/SLeeLa.git "$SLEELA_SRC"
fi

docker compose up -d --build

echo -n "waiting for http://localhost:${SITE_PORT}/healthz "
for _ in $(seq 1 60); do
  if curl -fsS "http://localhost:${SITE_PORT}/healthz" >/dev/null 2>&1; then echo "ok"; echo "SLeeLa site: http://localhost:${SITE_PORT}/"; exit 0; fi
  echo -n "."; sleep 2
done
echo; echo "site did not become healthy; check: docker compose logs site" >&2
exit 1
