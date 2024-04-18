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

for file in $(find "good" -name "*.tig"); do
  check "$file" 0 "--rename -bBeEAT"
done

for file in $(find "syntax" -name "*.tig"); do
  check "$file" 3 "-A"
done

for file in $(find "bind" -name "*.tig"); do
  check "$file" 4 "-bBA"
done

for file in $(find "type" -name "*.tig"); do
  check "$file" 5 "-T"
done

echo "» Total: $counter\n» Passed: $passed ($(($passed * 100 / $counter))%)"
