#include "../include/graph.hpp"
#include <random>

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_complete_graph(size_t n) {
    clear();
    for (size_t i = 0; i < n; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            add_edge(i, j, 1);
        }
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_cycle_graph(size_t n) {
    if (n < 3) throw std::invalid_argument("Cycle graph requires at least 3 vertices");
    
    clear();
    for (size_t i = 0; i < n; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < n - 1; ++i) {
        add_edge(i, i + 1, 1);
    }
    add_edge(n - 1, 0, 1);
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_path_graph(size_t n) {
    if (n < 2) throw std::invalid_argument("Path graph requires at least 2 vertices");
    
    clear();
    for (size_t i = 0; i < n; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < n - 1; ++i) {
        add_edge(i, i + 1, 1);
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_star_graph(size_t n) {
    if (n < 2) throw std::invalid_argument("Star graph requires at least 2 vertices");
    
    clear();
    for (size_t i = 0; i < n; ++i) {
        add_vertex(i);
    }
    for (size_t i = 1; i < n; ++i) {
        add_edge(0, i, 1);
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_grid_graph(size_t m, size_t n) {
    if (m < 2 || n < 2) throw std::invalid_argument("Grid dimensions must be at least 2x2");
    
    clear();
    for (size_t i = 0; i < m * n; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n - 1; ++j) {
            add_edge(i * n + j, i * n + j + 1, 1);
        }
    }
    for (size_t i = 0; i < m - 1; ++i) {
        for (size_t j = 0; j < n; ++j) {
            add_edge(i * n + j, (i + 1) * n + j, 1);
        }
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_hypercube_graph(size_t dimension) {
    size_t vertices = 1 << dimension; // 2^dimension
    
    clear();
    for (size_t i = 0; i < vertices; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
            size_t neighbor = i ^ (1 << j); 
            if (neighbor > i) { 
                add_edge(i, neighbor, 1);
            }
        }
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_connected_graph(size_t n, double edge_probability) {
    clear();
    
    for (size_t i = 0; i < n; ++i) {
        add_vertex(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (dis(gen) < edge_probability) {
                add_edge(i, j, 1); 
            }
        }
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_bipartite_graph(size_t m, size_t n, double edge_probability) {
    clear();
    
    for (size_t i = 0; i < m; ++i) {
        add_vertex(i);
    }
    
    for (size_t i = m; i < m + n; ++i) {
        add_vertex(i);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = m; j < m + n; ++j) {
            if (dis(gen) < edge_probability) {
                add_edge(i, j, 1); 
            }
        }
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_complete_bipartite_graph(size_t m, size_t n) {
    clear();
    for (size_t i = 0; i < m; ++i) {
        add_vertex(i);
    }
    for (size_t i = 0; i < n; ++i) {
        add_vertex(m + i);
    }
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            add_edge(i, m + j, 1);
        }
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_tree(size_t n) {
    if (n < 1) throw std::invalid_argument("Tree requires at least 1 vertex");
    
    clear();
    
    add_vertex(0);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, 0);
    
    for (size_t i = 1; i < n; ++i) {
        size_t parent = dis(gen) % i;
        
        add_vertex(i);
        
        add_edge(parent, i, 1);
    }
}
