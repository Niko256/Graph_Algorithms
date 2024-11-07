#!/bin/bash

PROJECT_DIR=$(dirname "$(dirname "$0")")

source "$PROJECT_DIR/visualization/venv/bin/activate"

manim -pqm "$PROJECT_DIR/visualization/$1"
