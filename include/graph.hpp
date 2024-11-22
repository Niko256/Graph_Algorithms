#pragma once
#include <cstddef>
#include <nlohmann/json_fwd.hpp>
#include "../../Data_Structures/Containers/Dynamic_Array.hpp"
#include "../../Data_Structures/SmartPtrs/include/SharedPtr.hpp"
#include <cerrno>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include "edge.hpp"
#include "vertex.hpp"


using json = nlohmann::json;


template <typename VertexId, typename Resource, typename WeightType>
class Graph {
private:
    std::unordered_map<VertexId, Vertex<VertexId, Resource>> vertex_pool_;

    DynamicArray<Edge<VertexId, WeightType>> edges_;
    
    std::unordered_map<VertexId, std::unordered_map<VertexId, SharedPtr<Edge<VertexId, WeightType>>>> adjacency_list_;

    size_t vertex_count_;

    json log_json_;
    
    void resize(size_t new_size);


public:

    Graph(const Graph& other);

    explicit Graph(size_t vertex_count);
    
    Graph& operator=(const Graph& other);
    
    Graph(Graph&& other) noexcept;

    Graph() : vertex_count_(0) {};

    Graph& operator=(Graph&& other) noexcept;

    ~Graph() = default;

    void initialize_graph(size_t n);

    void add_edge(VertexId from, VertexId to, WeightType weight);

    void add_vertex(VertexId id);
    void add_vertex(VertexId id, const Resource& data);

    void remove_edge(VertexId from, VertexId to);

    void remove_vertex(VertexId vertex);

    void reset_parameters();

    json to_json();

    void save_to_json(const std::string& filename);

    void load_from_json(const std::string& filename);

    void save_json_to_file(const std::string& filename, const json& data);

    const std::unordered_map<VertexId, std::unordered_map<VertexId, SharedPtr<Edge<VertexId, WeightType>>>>& get_adjacency_list() const; 
   
    bool operator==(const Graph& other) const;
    bool operator!=(const Graph& other) const;

    size_t get_degree(const VertexId& vertex) const;

    bool is_connected(const VertexId& from, const VertexId& to) const;

    void set_edge_weight(const VertexId& from, const VertexId& to, const WeightType& weight);

    const Vertex<VertexId, Resource>& get_vertex(const VertexId& id) const;

    const Edge<VertexId, WeightType>& get_edge(const VertexId& from, const VertexId& to) const; 
    
    const WeightType& get_edge_weight(const VertexId& from, const VertexId& to) const;
    
    const std::unordered_map<VertexId, Vertex<VertexId, Resource>>& get_vertices() const;

    const json get_json() const;

    bool has_vertex(const VertexId& vertex) const;

    bool has_edge(const VertexId& from, const VertexId& to) const;

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
    void depth_first_search(VertexId start); // DONE
    void breadth_first_search(VertexId start); // DONE

    // Connectivity
    DynamicArray<DynamicArray<VertexId>> find_connected_components(); // DONE 
    void find_bridges_and_articulation_points(); // TODO
    void find_biconnected_components(); // TODO

    // Shortest paths
    void dijkstra(VertexId start);
    void bellman_ford(VertexId start);
    void floyd_warshall();
    void shortest_paths_unweighted(VertexId start);

    // Maximum flow and minimum cut
    void ford_fulkerson(VertexId source, VertexId sink);
    void edmonds_karp(VertexId source, VertexId sink);
    void dinic(VertexId source, VertexId sink);
    void goldberg_tarjan(VertexId source, VertexId sink);
    void karger_stein();

    // Matchings
    void kuhn_max_matching();
    void edmonds_max_matching();
    void hopcroft_karp();

    // Trees
    void kruskal();
    void prim();
    void boruvka();
    void lca(VertexId u, VertexId v);
    void heavy_light_decomposition();


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
#include "../src/algorithms/dijkstra.tpp"
