#pragma once
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <concepts>
#include "../../Data_Structures/Pair.hpp"
#include "../../Data_Structures/Dynamic_Array.hpp"
#include "../../Data_Structures/DbList.hpp"

using Vertex = size_t;

template <typename W>
concept WeightType = std::is_floating_point_v<W> || std::is_integral_v<W>;

// adjacency list
template <WeightType Weight>
class Graph {
private:
    size_t vertex_count_;
    bool directed_;
    DynamicArray<DbList<Pair<Vertex, Weight>>> adjacency_list_;

public:

    using vertex_iterator_ = RandomAccessIterator<DbList<Pair<Vertex, Weight>>>;
    using c_vertex_iterator_ = ConstRandomAccessIterator<DbList<Pair<Vertex, Weight>>>; 
    using edge_iterator_ = BidirectionalIterator<Pair<Vertex, Weight>>;
    using c_edge_iterator_ = ConstBidirectionalIterator<Pair<Vertex, Weight>>;

    // -----------------------------------------------------------------------

    explicit Graph(size_t vertex_count, bool directed = false) 
        : vertex_count_(vertex_count), directed_(directed), adjacency_list_(vertex_count) {}

    Graph() : vertex_count_(0), directed_(false), adjacency_list_(nullptr) {}

    void add_vertex() noexcept;

    // void link_vertex() noexcept;

    void remove_vertex(Vertex vert_);

    void add_edge(Vertex from_, Vertex to_, Weight weight);

    bool has_edge(Vertex from_, Vertex to_) const;

    size_t vertex_count() const noexcept;

    const DbList<Pair<Vertex, Weight>>& get_adjacency_list(Vertex vert_) const;

    Weight get_edge_weight(Vertex from_, Vertex to_) const;

    void set_edge_weight(Vertex from_, Vertex to_, Weight weight);

    void add_undirected_edge(Vertex from_, Vertex to_, Weight weight);

    void remove_undirected_edge(Vertex from_, Vertex to_); 

    void remove_edge(Vertex from_, Vertex to_);


    vertex_iterator_ vertex_begin() const noexcept;
    vertex_iterator_ vertex_end() const noexcept;
    c_vertex_iterator_ vertex_begin() const noexcept;
    c_vertex_iterator_ vertex_end() const noexcept;

    edge_iterator_ edge_begin() const noexcept;
    edge_iterator_ edge_end() const noexcept;
    c_edge_iterator_ edge_begin() const noexcept;
    c_edge_iterator_ edge_end() const noexcept;

    // ---------------------------------------------------------------

    // Algorithms
    
    // Graph Traversal
    DynamicArray<Vertex> bfs(Vertex start_) const;
    DynamicArray<Vertex> dfs(Vertex start_) const;
    DynamicArray<Vertex> topological_sort() const;

    // Connectivity
    DynamicArray<DynamicArray<Vertex>> find_connected_components() const;
    DynamicArray<Pair<Vertex, Vertex>> find_bridges() const;
    DynamicArray<Vertex> find_articulation_points() const;
    DynamicArray<DynamicArray<Vertex>> kosaraju_scc() const;
    DynamicArray<DynamicArray<Vertex>> tarjan_scc() const;
    DynamicArray<DynamicArray<Vertex>> find_biconnected_components() const;

    // Shortest paths
    DynamicArray<Weight> dijkstra(Vertex start_) const;
    DynamicArray<Weight> bellman_ford(Vertex start_) const;
    DynamicArray<DynamicArray<Weight>> floyd_warshall() const;
    DynamicArray<DynamicArray<Weight>> jonson_all_pairs_shortest_paths() const;
    DynamicArray<Vertex> shortest_path_unweighted(Vertex from_, Vertex to_) const;

    // Maximum flow and maximum cut 
    Weight floyd_fulkerson(Vertex source_, Vertex sink_) const;
    Weight edmonds_karp(Vertex source_, Vertex sink_) const;
    Weight dinic(Vertex source_, Vertex sink_) const;
    Weight goldberg_tarjan(Vertex source_, Vertex sink_) const;
    Weight karger_stein(Vertex source_, Vertex sink_) const;

    // Matchings
    DynamicArray<Pair<Vertex, Vertex>> kuhn_max_matchings() const;
    DynamicArray<Pair<Vertex, Vertex>> edmonds_max_matching() const;
    DynamicArray<Pair<Vertex, Vertex>> hopcroft_karp_max_matching() const;

    // Trees
    Graph<Weight> kruskal_mst() const;
    Graph<Weight> prim_mst() const;
    Graph<Weight> boruvka_mst() const;
    Graph<Weight> lca(Vertex u_, Vertex v_) const;
    void heavy_light_decomposition() const;

    // Planarity
    bool is_planar() const;
    bool is_outerplanar() const;
    Graph<Weight> frucht_reingold_mst() const;

    // Network and flows
    Weight max_flow(Vertex source_, Vertex sink_) const;
    DynamicArray<Pair<Vertex, Vertex>> min_cut(Vertex source_, Vertex sink_) const;
    DynamicArray<Weight> min_cost_flow(Vertex source_, Vertex sink_, Weight max_flow_) const;
    DynamicArray<Pair<Vertex, Vertex>> min_cost_assignment() const;

    // Heuristic Algorithms
    Graph<Weight> greedy_mst() const;
    DynamicArray<Vertex> christofides_tsp() const;
    DynamicArray<Vertex> two_opt_tsp() const;

    // Random Graphs
    Graph<Weight> generate_erdos_renyi(size_t n_, double p_);
    Graph<Weight> generate_barabasi_albert(size_t n_, size_t m_);
    Graph<Weight> generate_random_graph(size_t n_, size_t m_);
    Graph<Weight> generate_regular_graph(size_t n_, size_t k_);
    Graph<Weight> generate_degree_distribution_graph(size_t n_, const DynamicArray<size_t>& degrees_);

    // Algorithms on Weighted Graphs
    DynamicArray<Weight> dijkstra_with_potentials(Vertex start_, const DynamicArray<Weight>& potentials_) const;
    DynamicArray<Weight> bellman_ford_with_negative_cycles(Vertex start_) const;
    DynamicArray<DynamicArray<Weight>> johnson_all_pairs_shortest_paths_with_potentials() const;

    // Algorithms on Directed Graphs
    DynamicArray<DynamicArray<Vertex>> tarjan_scc_directed() const;
    DynamicArray<DynamicArray<Vertex>> kosaraju_scc_directed() const;
    DynamicArray<DynamicArray<bool>> floyd_warshall_transitive_closure() const;

    // Traveling Salesman Problem (TSP)
    DynamicArray<Vertex> tsp_nearest_neighbor() const;
    DynamicArray<Vertex> tsp_two_opt() const;
    DynamicArray<Vertex> tsp_christofides() const;
    DynamicArray<Vertex> tsp_branch_and_bound() const;
    DynamicArray<Vertex> tsp_genetic_algorithm() const;

    // Multi-Objective Search
    DynamicArray<Vertex> epsilon_constrained_search(Vertex start_, const DynamicArray<Weight>& constraints_) const;
    DynamicArray<Vertex> pareto_front_search(Vertex start_) const;
    DynamicArray<Vertex> nsga_ii(Vertex start_) const;
    DynamicArray<Vertex> spea2(Vertex start_) const;

    // Knapsack Problem
    DynamicArray<Vertex> knapsack_greedy(Weight capacity_) const;
    DynamicArray<Vertex> knapsack_dynamic_programming(Weight capacity_) const;
    DynamicArray<Vertex> knapsack_branch_and_bound(Weight capacity_) const;
    DynamicArray<Vertex> knapsack_genetic_algorithm(Weight capacity_) const;

    // Graph Generation with Given Topology and Size
    Graph<Weight> generate_random_graph_with_topology(size_t n_, size_t m_, const DynamicArray<Pair<Vertex, Vertex>>& topology_);
    Graph<Weight> generate_regular_graph_with_degree(size_t n_, size_t k_);
    Graph<Weight> generate_graph_with_degree_distribution(size_t n_, const DynamicArray<size_t>& degrees_);
    Graph<Weight> generate_graph_with_diameter(size_t n_, size_t diameter_);
};

#include "../src/traversal.cpp"
#include "../src/connectivity.cpp"
#include "../src/network_and_flows.cpp"
#include "../src/tsp.cpp"
#include "../src/trees.cpp"
#include "../src/random.cpp"
#include "../src/topology.cpp"
#include "../src/weighted.cpp"
#include "../src/planarity.cpp"
#include "../src/heuristics.cpp"
#include "../src/shortest_paths.cpp"
#include "../src/graph_methods.cpp"
#include "../src/max_flow_max_cut.cpp"
#include "../src/matchings.cpp"
