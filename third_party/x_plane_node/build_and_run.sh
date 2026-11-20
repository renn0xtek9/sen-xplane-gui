#!/bin/bash
set -euo pipefail

cd -- "$(dirname -- "$0")"
cmake -S . -B build
cmake --build build
export LD_LIBRARY_PATH="$(pwd)/build/bin:${LD_LIBRARY_PATH:-}"
exec sen run config.yaml
