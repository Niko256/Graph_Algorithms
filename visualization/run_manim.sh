#!/bin/bash

PROJECT_DIR=$(dirname "$(dirname "$0")")

source "$PROJECT_DIR/visualization/venv/bin/activate"

if [ "$1" = "graph_visualization_static.py" ]; then
    manim -s -q k -p --format png "$PROJECT_DIR/visualization/$1"
else
    manim -pqm "$PROJECT_DIR/visualization/$1"
fi
