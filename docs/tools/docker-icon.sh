#!/bin/sh
command -v docker >/dev/null 2>&1 || { echo >&2 "The script requires 'docker' but it's not installed.  Aborting."; exit 1; }

#
DIR=$(dirname "$(readlink -f "$0")")
DIR_DOCS=$(dirname "$DIR")
DIR_ICON="$DIR_DOCS/icon"

mkdir -p "$DIR_ICON/images"
docker run --rm -v "$DIR_DOCS":/media/ jrbeverly/rsvg:privileged sh tools/icon-gen.sh