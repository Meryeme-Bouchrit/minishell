#!/bin/bash

echo "== Bash Parser Test =="

while IFS= read -r line; do
  echo ">>> INPUT: $line"
  bash -c "$line"
  echo "---------------------------"
done < parsing_tests.txt

