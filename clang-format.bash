#!/usr/bin/env bash
find ./include ./src ./test/ -type f \( -iname \*.cpp -o -iname \*.hpp \) | xargs clang-format -style="{ColumnLimit : 100}" -i
