#!/bin/sh
set -eu
n=$(find .. -maxdepth 2 -name '*.sleela' -type f | wc -l)
[ "$n" -ge 4 ]
printf 'SLeeLa video source inventory: %s files\n' "$n"
