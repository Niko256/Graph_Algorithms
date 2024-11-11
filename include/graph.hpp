#pragma once
#include <cstddef>
#include <nlohmann/json_fwd.hpp>
#include "../../Data_Structures/Containers/Dynamic_Array.hpp"
#include <cerrno>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include "edge.hpp"
#include "vertex.hpp"


using json = nlohmann::json;


template <typename VertexType, typename WeightType>
class Graph {
private:
    std::unordered_map<VertexType, std::unordered_map<VertexType, Edge<VertexType, WeightType>>> adjacency_list_;
    std::unordered_map<VertexType, Vertex<VertexType, WeightType>> vertices_;
    
    size_t vertex_count_;

    json log_json_;
    
    void resize(size_t new_size);
    void initialize_graph(size_t n);

public:
    Graph(const Graph& other);

    explicit Graph(size_t vertex_count);
    
    Graph& operator=(const Graph& other);
    
    Graph(Graph&& other) noexcept;

    Graph() : vertex_count_(0) {};

    Graph& operator=(Graph&& other) noexcept;

    ~Graph() = default;


    void add_edge(VertexType from, VertexType to, WeightType weight);

    void add_vertex(VertexType id);

    void remove_edge(VertexType from, VertexType to);

    void remove_vertex(VertexType vertex);


    json to_json() const;

    void save_to_json(const std::string& filename);

    void load_from_json(const std::string& filename);

    void save_json_to_file(const std::string& filename, const json& data);


    const std::unordered_map<VertexType, std::unordered_map<VertexType, Edge<VertexType, WeightType>>>& get_adjacency_list() const;
   
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;

    size_t get_degree(const VertexType& vertex) const;

    bool is_connected(const VertexType& from, const VertexType& to) const;

    void set_edge_weight(const VertexType& from, const VertexType& to, const WeightType& weight);

    const Vertex<VertexType, WeightType>& get_vertex(const VertexType& id) const;
    
    const Edge<VertexType, WeightType>& get_edge(const VertexType& from, const VertexType& to) const;
    
    const std::unordered_map<VertexType, Vertex<VertexType, WeightType>>& get_vertices() const;

    bool has_vertex(const VertexType& vertex) const;

    bool has_edge(const VertexType& from, const VertexType& to) const;

    size_t vertex_count() const noexcept;

    size_t edge_count() const noexcept;

    bool is_empty() const noexcept;
    
    void clear();

    auto begin() noexcept { return adjacency_list_.begin(); }
    auto end() noexcept { return adjacency_list_.end(); }
    auto cbegin() const noexcept { return adjacency_list_.cbegin(); }
    auto cend() const noexcept { return adjacency_list_.cend(); }


// Algorithms

    // Graph traversal
    void depth_first_search(VertexType start); // DONE
    void breadth_first_search(VertexType start); // DONE

    // Connectivity
    DynamicArray<DynamicArray<VertexType>> find_connected_components(); // DONE 
    void find_bridges_and_articulation_points(); // TODO
    void find_biconnected_components(); // TODO

    // Shortest paths
    void dijkstra(VertexType start);
    void bellman_ford(VertexType start);
    void floyd_warshall();
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

    // Algorithms on graphs with weights
    void dijkstra_with_potentials(VertexType start);
    void bellman_ford_with_negative_cycles(VertexType start);
    void johnson_all_pairs_shortest_paths();


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

    // Path search with the highest capacity
    void ford_fulkerson_max_capacity_path(VertexType source, VertexType sink);
    void dinic_max_capacity_path(VertexType source, VertexType sink);

    // Knapsack problem
    void knapsack_greedy();
    void knapsack_dynamic_programming();
    void knapsack_branch_and_bound();
    void knapsack_genetic_algorithm();


    // Graph Generators
    void generate_complete_graph(size_t n);
    void generate_cycle_graph(size_t n);
    void generate_path_graph(size_t n);
    void generate_star_graph(size_t n);
    void generate_grid_graph(size_t m, size_t n);
    void generate_hypercube_graph(size_t dimension);
    void generate_tree(size_t n);
    void generate_connected_graph(size_t n, double edge_probability);
    void generate_bipartite_graph(size_t m, size_t n, double edge_probability);
    void generate_complete_bipartite_graph(size_t m, size_t n);
};


#include "../src/graph.tpp"
#include "../src/algorithms/dfs.tpp"
#include "../src/algorithms/bfs.tpp"
#include "../src/algorithms/components.tpp"
#include "../src/generators.tpp"
