#include "discrete/discrete.hpp"

int
main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "usage: verifer [graph file] [solution file]" << std::endl;
        return -1;
    }

    const std::string graph_filename(argv[1]);
    const std::string sol_filename(argv[2]);

    const Graph graph = Graph::read(graph_filename.c_str());
    const auto fvs = FVS::read(sol_filename, graph.N());

    if (Graph::is_acyclic(graph, fvs)) {
        std::cout << "OK" << std::endl;
        return 0;
    }

    std::cerr << "Feedback vertex set does not induce an acyclic subgraph"
              << std::endl;
    std::exit(-1);
    return -1;
}
