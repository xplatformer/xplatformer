#!/bin/sh
set -ex

# Variables
#
# Build variables for directories.
SCRIPT=$(readlink -f "$0")
DIR="$(dirname $SCRIPT)"
ROOT_DIR="$(dirname $DIR)"
SRC_DIR="${ROOT_DIR}/src"
BIN_DIR="${ROOT_DIR}/output"

# Build
#
# Build the latex project
rm -rf ${BIN_DIR}
mkdir -p ${BIN_DIR}
cd ${SRC_DIR}/
make