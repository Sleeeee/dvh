#!/bin/bash

TEST_DIR="tests/renode/src"
IMAGE_NAME="antmicro/renode:latest"
OUTPUT_DIR="build/tests"

if [ ! -d "$TEST_DIR" ]; then
  echo "Error: unable to find the test directory $TEST_DIR"
  echo "Make sure you run this script from the firmware/ directory"
  exit 1
fi

echo "Running Renode tests..."

TEST_FILES="$TEST_DIR/*.robot"
if ! docker run --rm \
  -u $(id -u):$(id -g) \
  -v $(pwd):/project \
  -w /project \
  "$IMAGE_NAME" \
  renode-test -r "$OUTPUT_DIR" $TEST_FILES; then

  echo "Testing failed"
  exit 1
else
  echo "Testing successful"
  exit 0
fi
