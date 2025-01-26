#!/bin/sh
export WORKDIR=$(mktemp -d)

set -e
cc -Wall -Wextra -shared -o "${WORKDIR}/libion.so" ./src/*.c
sudo install "${WORKDIR}/libion.so" /usr/local/include
sudo install ./src/ion.h /usr/local/include
