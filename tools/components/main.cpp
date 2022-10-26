#include "discrete/discrete.hpp"

int
main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "wrong usage!" << std::endl;
        exit(1);
    }
    std::string filename = argv[1];

    SubGraph graph(Graph::read(argv[1]));

    auto [subgraphs, always_in, vc_components] = fully_reduce_graph(graph);

    index_t i = 0;
    for (auto& subgraph : subgraphs) {
        SubGraph::write(subgraph,
                        filename + std::string("_scc_") + std::to_string(i) +
                          std::string("_r"));
        i++;
    }
}