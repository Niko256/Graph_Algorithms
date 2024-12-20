#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"
BUILD_DIR="$PROJECT_ROOT/build"
VENV_DIR="$BUILD_DIR/venv"
VISUALIZATION_DIR="$PROJECT_ROOT/visualization"

if [ ! -f "$BUILD_DIR/benchmark_results.csv" ]; then
    echo "Error: benchmark_results.csv not found in build directory!"
    exit 1
fi

if [ ! -d "$VENV_DIR" ]; then
    python3 -m venv "$VENV_DIR"
fi

source "$VENV_DIR/bin/activate"

pip install pandas matplotlib seaborn adjustText

python3 "$VISUALIZATION_DIR/perfomance_plots.py"

deactivate

echo "Visualization complete!"
