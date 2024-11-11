#pragma once
#include "../../Data_Structures/SmartPtrs/include/UniquePtr.hpp"


template <typename T, typename Resource>
class Vertex {
private:
    T id_;
    UniquePtr<Resource> data_;

    bool visited_ = false;
    size_t color_ = 0;
    int discovery_time_ = -1;
    int finish_time_ = -1;

public:
    Vertex() : id_(), data_(nullptr) {}

    Vertex(T id);

    Vertex(T id, Resource data);

    Vertex(T id, const Resource& data);

    Vertex(T id, Resource&& data);

    Vertex& operator=(Vertex&& other);

    Vertex(const Vertex& other);

    Vertex(Vertex&& other) noexcept;

    ~Vertex();

    bool operator==(const Vertex& other) const;
    bool operator!=(const Vertex& other) const;

    T get_id() const;

    const UniquePtr<Resource>& get_data() const;
    size_t get_color() const;
    int get_discovery_time() const;
    int get_finish_time() const;

    void set_color(size_t other_color);
    void set_discovery_time(int d_time);
    void set_finish_time(int s_time);
};

#include "../src/vertex.tpp"
