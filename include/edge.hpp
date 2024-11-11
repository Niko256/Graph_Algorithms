#pragma once

template <typename T, typename W>
class Edge {
private:
    T from_;
    T to_;
    W weight_;
    bool is_bridge_ = false;

public:
    Edge() : from_(), to_(), weight_() {}

    Edge(T f, T t, W w);

    Edge(const Edge& other_edge);

    Edge& operator=(const Edge& other_edge);

    Edge(Edge&& other_edge) noexcept;

    Edge& operator=(Edge&& other_edge) noexcept;

    ~Edge();


    bool operator==(const Edge& other_edge) const;
    bool operator!=(const Edge& other_edge) const;

    T get_from() const;
    T get_to() const;
    W get_weight() const;

    void set_weight(W new_weight);

    bool is_bridge() const noexcept;
    void set_bridge(bool value = true);
};


#include "../src/edge.tpp"
