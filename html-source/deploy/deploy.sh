#!/usr/bin/env bash
# One-command Linux deploy with Docker: fetch/refresh the SLeeLa checkout, build the image, start the site.
#   ./deploy/deploy.sh                      # http://localhost:8080
#   SLEELA_SRC=/srv/sleela SITE_PORT=80 ./deploy/deploy.sh
# Rerunning pulls new content and recreates the container, so the site always shows the current checkout.
# For a host that already runs Apache + Tomcat, use deploy-apache-tomcat.sh instead.
set -euo pipefail
cd "$(dirname "$0")/.."

# default checkout location: /srv/sleela when we can write there, else ~/sleela (no sudo needed)
if [ -z "${SLEELA_SRC:-}" ]; then
  if [ -d /srv/sleela ] || [ -w /srv ] || [ "$(id -u)" -eq 0 ]; then SLEELA_SRC=/srv/sleela; else SLEELA_SRC="$HOME/sleela"; fi
fi
SITE_PORT="${SITE_PORT:-8080}"
export SLEELA_SRC SITE_PORT

for c in docker git curl; do command -v "$c" >/dev/null || { echo "$c is required" >&2; exit 1; }; done
docker compose version >/dev/null 2>&1 || { echo "the Docker Compose v2 plugin is required (docker compose)" >&2; exit 1; }

if [ -d "$SLEELA_SRC/.git" ]; then
  git -C "$SLEELA_SRC" pull --ff-only
elif [ -d "$SLEELA_SRC/tutorial" ]; then
  echo "using existing content at $SLEELA_SRC"
else
  mkdir -p "$(dirname "$SLEELA_SRC")"
  git clone --depth 1 https://github.com/mearvk/SLeeLa.git "$SLEELA_SRC"
fi

# --force-recreate: the site caches its file index at startup, so a content-only update needs a fresh container
docker compose up -d --build --force-recreate

echo -n "waiting for http://localhost:${SITE_PORT}/healthz "
for _ in $(seq 1 60); do
  if curl -fsS "http://localhost:${SITE_PORT}/healthz" >/dev/null 2>&1; then echo "ok"; echo "SLeeLa site: http://localhost:${SITE_PORT}/"; exit 0; fi
  echo -n "."; sleep 2
done
echo; echo "site did not become healthy; check: docker compose logs site" >&2
exit 1
