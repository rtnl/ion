#!/bin/sh
export WORKDIR=$(mktemp -d)

set -e

echo "running tests"
cc -DTEST -lcriterion -g -o "${WORKDIR}/libion_test" ./src/*.c
"${WORKDIR}/libion_test" --verbose

if [ -x "$(command -v valgrind)" ]; then
    echo "valgrind found"
    valgrind --trace-children=yes --leak-check=full -s "${WORKDIR}/libion_test"
else
    echo "valgrind not found"
fi
