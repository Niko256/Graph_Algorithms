#include <iostream>
#include "../include/graph.hpp"
#include <fstream>
#include <string>
#include <cstdlib>


int main() {
    Graph<int, int> graph(5);
    graph.add_edge(0, 1, 10);
    graph.add_edge(1, 2, 20);
    graph.add_edge(2, 3, 30);
    graph.add_edge(3, 4, 40);

    graph.depth_first_search(0);

    std::string command = "python3 ../visualization/visualize.py ../visualization/data/graph_data.txt";
    int result = std::system(command.c_str());

    if (result != 0) {
        std::cerr << "Error running visualization script" << std::endl;
    }

    return 0;
}
