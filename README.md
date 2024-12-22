An educational implementation of graph data structures and algorithms with interactive visualizations

## Project Overview

### Graph Implementation
- Template-based undirected weighted graph
- Average O(1) time complexity for basic operations (hash table-based storage)
- JSON serialization support
- Smart pointer-based memory management

### Graph Generators
- Complete Graph
- Path Graph
- Cycle Graph
- Star Graph
- Random Tree
- Bipartite Graph
- Grid Graph
- Hypercube Graph


### Visualization
Each algorithm includes animated visualization using Manim showing step-by-step execution

## Prerequisites

- C++20 compatible compiler
- CMake 3.30 or higher
- Python 3.8+ (for visualization)
- Dependencies:
  - Google Test
  - nlohmann_json 3.11.3+
  - Manim (installed automatically)

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

### Build tests
```bash
make tests
./tests
```

### Build project without tests
```bash
make graphs
./graphs
```
