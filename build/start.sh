#!/bin/sh
set -e

# Variables
#
# Build variables for directories.
SCRIPT=$(readlink -f "$0")
DIR="$(dirname $SCRIPT)"

# Environment
docker run --rm -it \
        -v $(dirname $DIR):/media \
        ubuntu:17.04 sh /media/build/provision.sh && sh