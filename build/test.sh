#!/bin/sh
export WORKDIR=$(mktemp -d)

set -e
cc -DTEST -lcriterion -o "${WORKDIR}/libion_test" ./src/*.c

echo "running tests"
"${WORKDIR}/libion_test" --verbose

if [ -x "$(command -v valgrind)" ]; then
    echo "valgrind found"
    valgrind "${WORKDIR}/libion_test"
else
    echo "valgrind not found"
fi
