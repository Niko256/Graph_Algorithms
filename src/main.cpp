#include <iostream>
#include "../include/graph.hpp"





int main() {
    Graph<int, int> g(6, false); 
    g.add_edge(0, 1, 4);
    g.add_edge(0, 2, 2);
    g.add_edge(1, 3, 5);
    g.add_edge(2, 4, 1);
    g.add_edge(3, 5, 3);
    g.add_edge(4, 5, 7);
    return 0;
}
