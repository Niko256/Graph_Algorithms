#pragma once
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include "../../Data_Structures/Pair.hpp"
#include "../../Data_Structures/Dynamic_Array.hpp"

template <typename VertexType, typename WeightType>
class Graph {
private:
    size_t vertex_count_; // Count of vertices
    bool directed_;
    DynamicArray<DynamicArray<Pair<VertexType, WeightType>>> adjacency_list_;

    void resize(size_t new_size) {
        if (new_size >= adjacency_list_.size()) {
            adjacency_list_.resize(new_size);
        }
    }

public:
    Graph() : vertex_count_(0), directed_(false) {}

    Graph(size_t vertex, bool dir = false) : vertex_count_(vertex), directed_(dir) {
        adjacency_list_.resize(vertex);
    }

    void add_edge(VertexType from, VertexType to, WeightType weight) {
        if (from >= vertex_count_ || to >= vertex_count_) {
            resize(std::max(from, to) + 1);
        }

        adjacency_list_[from].push_back(std::move(Pair(to, weight)));

        if (!directed_) {
            adjacency_list_[to].push_back(std::move(Pair(from, weight))); 
        }
    }

    void add_vertex() {
        if (vertex_count_ >= adjacency_list_.size()) {
            adjacency_list_.resize(2 * vertex_count_);
        }
        ++vertex_count_;
    }


    void remove_edge(VertexType from, VertexType to) {
        if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

        auto& edges = adjacency_list_[from];
        for (auto it = edges.begin(); it != edges.end(); ++it) {
            if ((*it).first_ == to) {
                edges.erase(it);
                break;
            }
        }

        if (!directed_) {
            auto& reversed_edges = adjacency_list_[to];
            for (auto it = reversed_edges.begin(); it != reversed_edges.end(); ++it) {
                if ((*it).first_ == from) {
                    reversed_edges.erase(it);
                    break;
                }
            }
        }
    }

   
    void remove_vertex(VertexType vertex) {
        if (vertex >= vertex_count_) throw std::out_of_range("out of range");

        for (size_t i = 0; i < vertex_count_; ++i) {
            if (i != vertex) {
                auto& edges = adjacency_list_[i];
                for (auto it = edges.begin(); it != edges.end(); ) {
                    if ((*it).first_ == vertex) {
                        it = edges.erase(it);
                    } else {
                        ++it;
                    }
                }
            }
        }

        adjacency_list_[vertex].clear();

        for (size_t i = 0; i < vertex_count_; ++i) {
            auto& edges = adjacency_list_[i];
            for (auto& edge : edges) {
                if (edge.first_ > vertex) {
                    --edge.first_;
                }
            }
        }

        for (size_t i = vertex; i < vertex_count_ - 1; ++i) {
            adjacency_list_[i] = std::move(adjacency_list_[i + 1]);
        }
        adjacency_list_.resize(vertex_count_ - 1);

        --vertex_count_;
    }


    bool has_edge(VertexType from, VertexType to) const {
        if (from >= vertex_count_ || to >= vertex_count_) throw std::out_of_range("out of range");

        const auto& edges = adjacency_list_[from];
        for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
            if ((*it).first_ == to) return true;
        }
        return false;

    }


    const DynamicArray<Pair<VertexType, WeightType>>& get_adjacency_list(VertexType vertex) const {
        return adjacency_list_[vertex];
    }


    constexpr size_t vertex_count() const noexcept { return vertex_count_; }

    constexpr bool is_directed() const noexcept { return directed_; }

    void clear() {
        for (size_t i = 0; i < vertex_count_; ++i) {
            adjacency_list_[i].clear();
        } 
        vertex_count_ = 0;
    }


    // Iterations
    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator begin() noexcept {
        return adjacency_list_.begin();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator cbegin() const noexcept {
        return adjacency_list_.cbegin();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::iterator end() noexcept {
        return adjacency_list_.end();
    }

    typename DynamicArray<DynamicArray<Pair<VertexType, WeightType>>>::const_iterator cend() const noexcept {
        return adjacency_list_.cend();
    }

// Algorithms
    
    // Graph Traversal
    DynamicArray<VertexType> bfs(VertexType start) const;
    DynamicArray<VertexType> dfs(VertexType start) const;
    DynamicArray<VertexType> topological_sort() const;

    // Connectivity
    DynamicArray<DynamicArray<VertexType>> find_connected_components() const;
    DynamicArray<Pair<VertexType, VertexType>> find_bridges() const;
    DynamicArray<VertexType> find_articulation_points() const;
    DynamicArray<DynamicArray<VertexType>> kosaraju_scc() const;
    DynamicArray<DynamicArray<VertexType>> tarjan_scc() const;
    DynamicArray<DynamicArray<VertexType>> find_biconnected_components() const;

    // Shortest paths
    DynamicArray<WeightType> dijkstra(VertexType start) const;
    DynamicArray<WeightType> bellman_ford(VertexType start) const;
    DynamicArray<DynamicArray<WeightType>> floyd_warshall() const;
    DynamicArray<DynamicArray<WeightType>> jonson_all_pairs_shortest_paths() const;
    DynamicArray<VertexType> shortest_path_unweighted(VertexType from, VertexType to) const;

    // Maximum flow and maximum cut 
    WeightType floyd_fulkerson(VertexType source, VertexType sink) const;
    WeightType edmonds_karp(VertexType source, VertexType sink) const;
    WeightType dinic(VertexType source, VertexType sink) const;
    WeightType goldberg_tarjan(VertexType source, VertexType sink) const;
    WeightType karger_stein(VertexType source, VertexType sink) const;

    // Matchings
    DynamicArray<Pair<VertexType, VertexType>> kuhn_max_matchings() const;
    DynamicArray<Pair<VertexType, VertexType>> edmonds_max_matching() const;
    DynamicArray<Pair<VertexType, VertexType>> hopcroft_karp_max_matching() const;

    // Trees
    Graph<VertexType, WeightType> kruskal_mst() const;
    Graph<VertexType, WeightType> prim_mst() const;
    Graph<VertexType, WeightType> boruvka_mst() const;
    Graph<VertexType, WeightType> lca(VertexType u, VertexType v) const;
    void heavy_light_decomposition() const;

    // Planarity
    bool is_planar() const;
    bool is_outerplanar() const;
    Graph<VertexType, WeightType> frucht_reingold_mst() const;

    // Network and flows
    WeightType max_flow(VertexType source, VertexType sink) const;
    DynamicArray<Pair<VertexType, VertexType>> min_cut(VertexType source, VertexType sink) const;
    DynamicArray<WeightType> min_cost_flow(VertexType source, VertexType sink, WeightType max_flow) const;
    DynamicArray<Pair<VertexType, VertexType>> min_cost_assignment() const;

    // Heuristic Algorithms
    Graph<VertexType, WeightType> greedy_mst() const;
    DynamicArray<VertexType> christofides_tsp() const;
    DynamicArray<VertexType> two_opt_tsp() const;

    // Random Graphs
    Graph<VertexType, WeightType> generate_erdos_renyi(size_t n, double p);
    Graph<VertexType, WeightType> generate_barabasi_albert(size_t n, size_t m);
    Graph<VertexType, WeightType> generate_random_graph(size_t n, size_t m);
    Graph<VertexType, WeightType> generate_regular_graph(size_t n, size_t k);
    Graph<VertexType, WeightType> generate_degree_distribution_graph(size_t n, const DynamicArray<size_t>& degrees);

    // Algorithms on WeightTypeed Graphs
    DynamicArray<WeightType> dijkstra_with_potentials(VertexType start, const DynamicArray<WeightType>& potentials) const;
    DynamicArray<WeightType> bellman_ford_with_negative_cycles(VertexType start) const;
    DynamicArray<DynamicArray<WeightType>> johnson_all_pairs_shortest_paths_with_potentials() const;

    // Algorithms on Directed Graphs
    DynamicArray<DynamicArray<VertexType>> tarjan_scc_directed() const;
    DynamicArray<DynamicArray<VertexType>> kosaraju_scc_directed() const;
    DynamicArray<DynamicArray<bool>> floyd_warshall_transitive_closure() const;

    // Traveling Salesman Problem (TSP)
    DynamicArray<VertexType> tsp_nearest_neighbor() const;
    DynamicArray<VertexType> tsp_two_opt() const;
    DynamicArray<VertexType> tsp_christofides() const;
    DynamicArray<VertexType> tsp_branch_and_bound() const;
    DynamicArray<VertexType> tsp_genetic_algorithm() const;

    // Multi-Objective Search
    DynamicArray<VertexType> epsilon_constrained_search(VertexType start, const DynamicArray<WeightType>& constraints) const;
    DynamicArray<VertexType> pareto_front_search(VertexType start) const;
    DynamicArray<VertexType> nsga_ii(VertexType start) const;
    DynamicArray<VertexType> spea2(VertexType start) const;

    // Knapsack Problem
    DynamicArray<VertexType> knapsack_greedy(WeightType capacity) const;
    DynamicArray<VertexType> knapsack_dynamic_programming(WeightType capacity) const;
    DynamicArray<VertexType> knapsack_branch_and_bound(WeightType capacity) const;
    DynamicArray<VertexType> knapsack_genetic_algorithm(WeightType capacity) const;

    // Graph Generation with Given Topology and Size
    Graph<VertexType, WeightType> generate_random_graph_with_topology(size_t n, size_t m, const DynamicArray<Pair<VertexType, VertexType>>& topology);
    Graph<VertexType, WeightType> generate_regular_graph_with_degree(size_t n, size_t k);
    Graph<VertexType, WeightType> generate_graph_with_degree_distribution(size_t n, const DynamicArray<size_t>& degrees);
    Graph<VertexType, WeightType> generate_graph_with_diameter(size_t n, size_t diameter);
};
