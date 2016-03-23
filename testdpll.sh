#!/bin/bash
for i in testsat/*; do echo -e "\n\033[0;31m$i :\033[0m\n" && ./dpll "$i" "$1";done
