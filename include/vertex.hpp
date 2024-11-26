#pragma once
#include "../dependencies/Data_Structures/SmartPtrs/include/UniquePtr.hpp"


template <typename VertexId, typename Resource>
class Vertex {
private:
    VertexId id_;
    UniquePtr<Resource> data_;

    bool visited_ = false;
    size_t color_ = 0;
    int discovery_time_ = -1;
    int finish_time_ = -1;

public:
    Vertex() : id_(), data_(nullptr) {}

    Vertex(VertexId id);

    Vertex(VertexId id, const Resource& data);

    Vertex(VertexId id, Resource&& data);

    Vertex& operator=(Vertex&& other);

    Vertex(const Vertex& other);

    Vertex(Vertex&& other) noexcept;

    ~Vertex();

    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;

    const VertexId& get_id() const;

    const UniquePtr<Resource>& get_data() const;
    size_t get_color() const;
    int get_discovery_time() const;
    int get_finish_time() const;

    void set_color(size_t other_color);
    void set_discovery_time(int d_time);
    void set_finish_time(int s_time);
    void set_data(const UniquePtr<Resource>& data);
};

#include "../src/vertex.tpp"
