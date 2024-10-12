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
 
    // Graph traversal
    void depth_first_search(VertexType start);
    void breadth_first_search(VertexType start);
    void topological_sort();
    void dfs_with_timing(VertexType start);

    // Connectivity
    void find_connected_components();
    void find_bridges_and_articulation_points();
    void kosaraju_scc();
    void find_biconnected_components();

    // Shortest paths
    void dijkstra(VertexType start);
    void bellman_ford(VertexType start);
    void floyd_warshall();
    void johnson();
    void shortest_paths_unweighted(VertexType start);

    // Maximum flow and minimum cut
    void ford_fulkerson(VertexType source, VertexType sink);
    void edmonds_karp(VertexType source, VertexType sink);
    void dinic(VertexType source, VertexType sink);
    void goldberg_tarjan(VertexType source, VertexType sink);
    void karger_stein();

    // Matchings
    void kuhn_max_matching();
    void edmonds_max_matching();
    void hopcroft_karp();

    // Trees
    void kruskal();
    void prim();
    void boruvka();
    void lca(VertexType u, VertexType v);
    void heavy_light_decomposition();

    // Planarity
    void kuratowski_planarity_test();
    void outer_planarity_test();
    void fruchterman_reingold();

    // Network and flows
    void max_flow_in_network(VertexType source, VertexType sink);
    void min_cut_in_network(VertexType source, VertexType sink);
    void min_cost_flow(VertexType source, VertexType sink);
    void assignment_problem();

    // Heuristic algorithms
    void greedy_algorithms();
    void christofides_algorithm();
    void local_search();

    // Random graphs
    void erdos_renyi_model();
    void barabasi_albert_model();
    void random_graph_generation();
    void random_graph_properties();

    // Algorithms on dynamic graphs
    void dynamic_mst();
    void dynamic_connectivity();
    void dynamic_shortest_paths();

    // Algorithms on graphs with constraints
    void max_flow_with_capacity_constraints(VertexType source, VertexType sink);
    void max_flow_with_time_constraints(VertexType source, VertexType sink);
    void max_flow_with_cost_constraints(VertexType source, VertexType sink);

    // Algorithms on graphs with weights
    void dijkstra_with_potentials(VertexType start);
    void bellman_ford_with_negative_cycles(VertexType start);
    void johnson_all_pairs_shortest_paths();

    // Algorithms on graphs with orientation
    void tarjan_scc();
    void kosaraju_scc_oriented();
    void floyd_warshall_transitive_closure();

    // Algorithms on graphs with time constraints
    void dijkstra_with_time_constraints(VertexType start);
    void bellman_ford_with_time_constraints(VertexType start);
    void floyd_warshall_with_time_constraints();

    // Algorithms on graphs with cost constraints
    void dijkstra_with_cost_constraints(VertexType start);
    void bellman_ford_with_cost_constraints(VertexType start);
    void floyd_warshall_with_cost_constraints();

    // Algorithms on graphs with capacity constraints
    void ford_fulkerson_with_capacity_constraints(VertexType source, VertexType sink);
    void edmonds_karp_with_capacity_constraints(VertexType source, VertexType sink);
    void dinic_with_capacity_constraints(VertexType source, VertexType sink);

    // Algorithms on graphs with time and cost constraints
    void dijkstra_with_time_and_cost_constraints(VertexType start);
    void bellman_ford_with_time_and_cost_constraints(VertexType start);
    void floyd_warshall_with_time_and_cost_constraints();

    // Algorithms on graphs with time, cost, and capacity constraints
    void dijkstra_with_time_cost_and_capacity_constraints(VertexType start);
    void bellman_ford_with_time_cost_and_capacity_constraints(VertexType start);
    void floyd_warshall_with_time_cost_and_capacity_constraints();

    // Algorithms on graphs with time, cost, capacity, and orientation constraints
    void dijkstra_with_time_cost_capacity_and_orientation_constraints(VertexType start);
    void bellman_ford_with_time_cost_capacity_and_orientation_constraints(VertexType start);
    void floyd_warshall_with_time_cost_capacity_and_orientation_constraints();

    // Traveling Salesman Problem
    void nearest_neighbor_tsp();
    void two_opt_tsp();
    void christofides_tsp();
    void branch_and_bound_tsp();
    void genetic_algorithm_tsp();

    // Multi-criteria search
    void epsilon_constrained_search();
    void pareto_front_search();
    void nsga_ii();
    void spea2();

    // Implementation of the "Lattice" ADT
    void lattice_operations();
    void lattice_path_search();
    void lattice_with_obstacles();
    void lattice_with_weights();

    // Path search with the highest capacity
    void ford_fulkerson_max_capacity_path(VertexType source, VertexType sink);
    void dinic_max_capacity_path(VertexType source, VertexType sink);

    // Knapsack problem
    void knapsack_greedy();
    void knapsack_dynamic_programming();
    void knapsack_branch_and_bound();
    void knapsack_genetic_algorithm();

    // Graph generalization with given topology and size
    void generate_random_graph_with_topology();
    void generate_regular_graph();
    void generate_graph_with_degree_distribution();
    void generate_graph_with_diameter();
};
