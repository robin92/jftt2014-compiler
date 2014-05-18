#!/usr/bin/env bash

lines="";

while read line
do
  lines="$lines$(echo $line | cut -d ';' -f -1)\n"
done < "${1:-/proc/${$}/fd/0}"

echo -ne $lines

