An educational implementation of an undirected graph data structure with algorithms and visualizations.

## Project Overview

This project implements an efficient undirected graph with the following characteristics:

- Template-based implementation
- O(1) amortized time complexity for basic operations (due to hash table-based storage)
- Visualization of algorithms using [Manim](https://github.com/3b1b/manim) animation engine

## Prerequisites

- C++20 compatible compiler
- CMake 3.30 or higher
- Python 3.8+ (for visualization)
- Google Test
- nlohmann_json 3.11.3+

## Installation

**Important**: This repository uses git submodules. Clone with:

```bash
git clone --recursive git@github.com:Niko256/Graph_Algorithms.git
```

Then build:

```bash
mkdir build && cd build
cmake ..
make
```
