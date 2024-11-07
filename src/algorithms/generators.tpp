#include "../../include/graph.hpp"
#include <random>
#include <cmath>


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_complete_graph(size_t n) {
    initialize_graph(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            add_edge(i, j, 1);
        }
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_path_graph(size_t n) {
    initialize_graph(n);

    for (size_t i = 0; i < n - 1; ++i) {
        add_edge(i, i + 1, 1);
    }
}

template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_cycle_graph(size_t n) {
    initialize_graph(n);

    for (size_t i = 0; i < n; ++i) {
        add_edge(i, (i + 1) % n, 1);
    }
}



template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_star_graph(size_t n) {
    initialize_graph(n);

    for (size_t i = 1; i < n; ++i) {
        add_edge(0, i, 1);
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_grid_graph(size_t m, size_t n) {
    size_t total_vertices = m * n;
    initialize_graph(total_vertices);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            size_t current = i * n + j;
            if (j + 1 < n) add_edge(current, current + 1, 1);  
            if (i + 1 < m) add_edge(current, current + n, 1);
        }
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_hypercube_graph(size_t dimension) {
    size_t n = 1 << dimension;  // 2^dimension
    initialize_graph(n);
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
            size_t neighbor = i ^ (1 << j);  
            if (i < neighbor) {
                add_edge(i, neighbor, 1);
            }
        }
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_tree(size_t n) {
    initialize_graph(n);
    
    if (n <= 1) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (size_t i = 1; i < n; ++i) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t parent = dist(gen);
        add_edge(parent, i, 1);
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_bipartite_graph(size_t m, size_t n, double edge_probability) {
    if (edge_probability < 0.0 || edge_probability > 1.0) {
        throw std::invalid_argument("Edge probability must be between 0 and 1");
    }
    
    initialize_graph(m + n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (dist(gen) < edge_probability) {
                add_edge(i, m + j, 1);
            }
        }
    }
}


template <typename VertexType, typename WeightType>
void Graph<VertexType, WeightType>::generate_complete_bipartite_graph(size_t m, size_t n) {
    initialize_graph(m + n);

    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            add_edge(i, m + j, 1);
        }
    }
}
