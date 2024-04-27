#!/bin/sh

tc="./../src/tc"

RED='\033[0;31m'
GREEN='\033[0;32m'
GRAY='\033[0;37m'
BLUE='\033[0;34m'
NC='\033[0m'

counter=0
passed=0

check() {
    local file="$1"

    counter=$(($counter + 1))

    "$tc" $3 "$1" 1> /dev/null 2> /dev/null

    if [ $? -eq $2 ]; then
      echo "${GREEN}✓${NC} $tc $3 $file"
      passed=$(($passed + 1))
    else
      echo "${RED}✗${NC} $tc $3 $file"
    fi
}

check_clang() {
  clang "-m32" "-otest" "/tmp/runtime-result.ll" 1> /dev/null 2> /dev/null

  if [ $? -eq 0 ]; then
    echo "${GREEN}✓${NC} compilation successful for $file"
    passed=$(($passed + 1))
  else
    echo "${RED}✗${NC} clang issue for $file"
  fi
}

check_llvm() {
    local file="$1"

    counter=$(($counter + 1))

    "$tc" "--llvm-runtime-display" "--llvm-display" $file 1> "/tmp/runtime-result.ll" 2> /dev/null

    if [ $? -eq 0 ]; then
      check_clang
    else
      echo "${RED}✗${NC} computing llvm for $file"
    fi
}

for file in $(find "good" -name "*.tig"); do
  check "$file" 0 "--rename -bBeEAT"
done

for file in $(find "syntax" -name "*.tig"); do
  check "$file" 3 "-A"
done

for file in $(find "lexical" -name "*.tig"); do
  check "$file" 2 "-A"
done

for file in $(find "bind" -name "*.tig"); do
  check "$file" 4 "-bBA"
done

for file in $(find "type" -name "*.tig"); do
  check "$file" 5 "-T"
done

for file in $(find "good" -name "*.tig"); do
  check_llvm "$file"
done

if [ $passed -eq $counter ]; then
  emote=" °˖✧◝(⁰▿⁰)◜✧˖°"
else
  emote=""
fi

echo "» Passed: $passed ($(($passed * 100 / $counter))%)$emote\n» Total: $counter"
