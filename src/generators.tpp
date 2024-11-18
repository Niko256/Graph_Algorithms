#include "../include/graph.hpp"
#include <random>


template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_complete_graph(size_t n) {
    initialize_graph(n);
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(new Edge<VertexId, WeightType>(i, j, 1));
            edges_.push_back(*edge_ptr);
            adjacency_list_[i][j] = edge_ptr;
            adjacency_list_[j][i] = edge_ptr;
        }
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_cycle_graph(size_t n) {
    if (n < 3) throw std::invalid_argument("Cycle graph requires at least 3 vertices");
    
    initialize_graph(n);
    
    for (size_t i = 0; i < n; ++i) {
        auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
            new Edge<VertexId, WeightType>(i, (i + 1) % n, 1)
        );
        edges_.push_back(*edge_ptr);
        adjacency_list_[i][(i + 1) % n] = edge_ptr;
        adjacency_list_[(i + 1) % n][i] = edge_ptr;
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_path_graph(size_t n) {
    if (n < 2) throw std::invalid_argument("Path graph requires at least 2 vertices");
    
    initialize_graph(n);
    
    for (size_t i = 0; i < n - 1; ++i) {
        auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
            new Edge<VertexId, WeightType>(i, i + 1, 1)
        );
        edges_.push_back(*edge_ptr);
        adjacency_list_[i][i + 1] = edge_ptr;
        adjacency_list_[i + 1][i] = edge_ptr;
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_star_graph(size_t n) {
    if (n < 2) throw std::invalid_argument("Star graph requires at least 2 vertices");
    
    initialize_graph(n);
    
    for (size_t i = 1; i < n; ++i) {
        auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
            new Edge<VertexId, WeightType>(0, i, 1)
        );
        edges_.push_back(*edge_ptr);
        adjacency_list_[0][i] = edge_ptr;
        adjacency_list_[i][0] = edge_ptr;
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_grid_graph(size_t m, size_t n) {
    if (m == 0 || n == 0) throw std::invalid_argument("Grid dimensions must be positive");
    
    size_t total_vertices = m * n;
    initialize_graph(total_vertices);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            size_t current = i * n + j;
            
            if (j + 1 < n) {
                auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
                    new Edge<VertexId, WeightType>(current, current + 1, 1)
                );
                edges_.push_back(*edge_ptr);
                adjacency_list_[current][current + 1] = edge_ptr;
                adjacency_list_[current + 1][current] = edge_ptr;
            }
            
            if (i + 1 < m) {
                auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
                    new Edge<VertexId, WeightType>(current, current + n, 1)
                );
                edges_.push_back(*edge_ptr);
                adjacency_list_[current][current + n] = edge_ptr;
                adjacency_list_[current + n][current] = edge_ptr;
            }
        }
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_hypercube_graph(size_t dimension) {
    size_t n = 1 << dimension;  // 2^dimension
    initialize_graph(n);
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < dimension; ++j) {
            size_t neighbor = i ^ (1 << j);
            if (i < neighbor) {
                auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
                    new Edge<VertexId, WeightType>(i, neighbor, 1)
                );
                edges_.push_back(*edge_ptr);
                adjacency_list_[i][neighbor] = edge_ptr;
                adjacency_list_[neighbor][i] = edge_ptr;
            }
        }
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_tree(size_t n) {
    initialize_graph(n);
    
    if (n <= 1) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for (size_t i = 1; i < n; ++i) {
        std::uniform_int_distribution<size_t> dist(0, i - 1);
        size_t parent = dist(gen);
        
        auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
            new Edge<VertexId, WeightType>(parent, i, 1)
        );
        edges_.push_back(*edge_ptr);
        adjacency_list_[parent][i] = edge_ptr;
        adjacency_list_[i][parent] = edge_ptr;
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_bipartite_graph(size_t m, size_t n, double edge_probability) {
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
                auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
                    new Edge<VertexId, WeightType>(i, m + j, 1)
                );
                edges_.push_back(*edge_ptr);
                adjacency_list_[i][m + j] = edge_ptr;
                adjacency_list_[m + j][i] = edge_ptr;
            }
        }
    }
}

template <typename VertexId, typename Resource, typename WeightType>
void Graph<VertexId, Resource, WeightType>::generate_complete_bipartite_graph(size_t m, size_t n) {
    initialize_graph(m + n);
    
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            auto edge_ptr = SharedPtr<Edge<VertexId, WeightType>>(
                new Edge<VertexId, WeightType>(i, m + j, 1)
            );
            edges_.push_back(*edge_ptr);
            adjacency_list_[i][m + j] = edge_ptr;
            adjacency_list_[m + j][i] = edge_ptr;
        }
    }
}
