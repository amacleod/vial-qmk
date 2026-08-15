#!/bin/sh
# Local docker-based build helper for this fork.
#
# util/docker_build.sh (upstream QMK's own helper) doesn't work as-is here:
#  - it doesn't pass an SELinux volume label, so podman/docker on an
#    SELinux-enforcing host (e.g. Fedora) can't read the mounted repo
#  - the ghcr.io/qmk/qmk_cli image needs `pip install -r requirements.txt`
#    run once per container, since the baked-in venv is missing deps our
#    checked-out tree needs (e.g. `appdirs`)
#
# Usage: util/docker_build_local.sh <keyboard>:<keymap>[:target]
#   e.g. util/docker_build_local.sh fingerpunch/kucheza:vial
#        util/docker_build_local.sh fingerpunch/kucheza:vial:flash
#
# Runs `make` niced and capped at half the machine's cores, so a build
# doesn't compete with anything else (e.g. gaming) running at the same time.

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <keyboard>:<keymap>[:target]" >&2
    exit 1
fi

RUNTIME="${RUNTIME:-docker}"
JOBS=$(( $(nproc) / 2 ))
[ "$JOBS" -lt 1 ] && JOBS=1

"$RUNTIME" run --rm -w /qmk_firmware -v "$(pwd)":/qmk_firmware:z ghcr.io/qmk/qmk_cli:latest \
    sh -c "/opt/uv/tools/qmk/bin/python3 -m pip install -q -r requirements.txt && nice -n 10 make $1 -j$JOBS"

# The container runs as root, so anything it wrote (build artifacts, the
# .uf2/.bin) ends up root-owned on the host -- hand it back.
"$RUNTIME" run --rm -w /qmk_firmware -v "$(pwd)":/qmk_firmware:z ghcr.io/qmk/qmk_cli:latest \
    chown -R "$(id -u):$(id -g)" /qmk_firmware
