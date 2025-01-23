#!/bin/sh
export WORKDIR=$(mktemp -d)

set -e
cc -DTEST -lcriterion -o "${WORKDIR}/libion_test" ./src/*.c
"${WORKDIR}/libion_test" --verbose
