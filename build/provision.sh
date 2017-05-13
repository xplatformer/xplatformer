#!/bin/sh
set -e

## Note:
##
## This build process is temporary until an alpine image
## to compile XPlatformer is properly configured
echo "This build process is temporary.  See jrbeverly/xwindow"

apt-get update -y
apt-get install -y make g++ gcc libx11-dev mesa-common-dev libglu1-mesa-dev libxrandr-dev libxi-dev