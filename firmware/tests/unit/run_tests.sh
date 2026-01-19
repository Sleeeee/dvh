#!/bin/bash

UNITY_SRC="tests/unit/lib/unity.c"
TEST_DIR="tests/unit/src"
BINARY="$TEST_DIR/test_binary"

if [ ! -d "$TEST_DIR" ]; then
  echo "Error: unable to find the test directory $TEST_DIR"
  echo "Make sure you run this script from the firmware/ directory"
  exit 1
fi

echo "Running unit tests..."
TOTAL_TESTS=0
FAILED_TESTS=0

for test_file in "$TEST_DIR"/*.c; do
  ((TOTAL_TESTS++))
  echo "Compiling $test_file..."

  if ! gcc -o "$BINARY" "$test_file" "$UNITY_SRC"; then
    echo "Error: Compilation failed"
    ((FAILED_TESTS++))
    continue
  fi

  echo "Binary compiled successfully"
  chmod u+x "$BINARY"
  echo "Running tests on $test_file..."

  if ! ./"$BINARY"; then
    ((FAILED_TESTS++))
  fi
done

echo "Cleaning up binary..."
rm "$BINARY"

echo "Test results: $TOTAL_TESTS total, $((TOTAL_TESTS - FAILED_TESTS)) passed"

if [ $FAILED_TESTS -eq 0 ]; then
  echo "Testing successful"
  exit 0
else
  echo "Testing failed"
  exit 1
fi
