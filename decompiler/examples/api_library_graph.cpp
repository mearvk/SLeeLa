#include "sleela/decompiler/library.hpp"
#include <iostream>

using namespace sleela::decompiler;

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: slecompiler-api-graph <artifact> [artifact ...]\n";
        return 2;
    }

    try {
        LibraryGraph graph;
        for (int i = 1; i < argc; ++i)
            graph.add(Artifact::open(argv[i]));

        std::cout << "nodes: " << graph.nodes().size() << "\n";
        for (const auto& node : graph.nodes()) {
            std::cout << node.name << " soname=" << node.soname
                      << " exports=" << node.exports.size() << "\n";
        }

        std::cout << "edges: " << graph.edges().size() << "\n";
        for (const auto& edge : graph.edges())
            std::cout << edge.consumer << " -> " << edge.provider
                      << " [" << edge.symbol << "]\n";
    } catch (const std::exception& e) {
        std::cerr << "Slecompiler error: " << e.what() << "\n";
        return 1;
    }
}
