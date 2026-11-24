#!/usr/bin/env bash
# Keymap unit tests — zero QMK toolchain required, just a C compiler.
# Mirrors the qmk_notifier module's run_all_tests.sh pattern.
set -euo pipefail
cd "$(dirname "$0")"

BIN=$(mktemp /tmp/test_vim_lazy_insert.XXXXXX)
trap 'rm -f "$BIN"' EXIT

cc -std=c11 -Wall -Wextra -Werror -o "$BIN" test_vim_lazy_insert.c
"$BIN"
