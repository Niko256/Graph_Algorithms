#pragma once

template <typename VertexId, typename W>
class Edge {
private:
    VertexId from_;
    VertexId to_;
    W weight_;
    bool is_bridge_ = false;

public:
    Edge() : from_(), to_(), weight_() {}

    Edge(VertexId f, VertexId t, W w);

    Edge(const Edge& other_edge);

    Edge& operator=(const Edge& other_edge);

    Edge(Edge&& other_edge) noexcept;

    Edge& operator=(Edge&& other_edge) noexcept;

    ~Edge();


    bool operator==(const Edge& other_edge) const;
    bool operator!=(const Edge& other_edge) const;

    const VertexId& get_from() const;
    const VertexId& get_to() const;
    const W get_weight() const;

    void set_weight(W new_weight);

    bool is_bridge() const noexcept;
    void set_bridge(bool value = true);
};


#include "../src/edge.tpp"
