#include <math.h>
#include <nlohmann/json.hpp>
#include <ogdf/basic/Graph_d.h>
#include <ogdf/basic/extended_graph_alg.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/basic/simple_graph_alg.h>
#include <ogdf/energybased/FMMMLayout.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/SugiyamaLayout.h>
#include <set>
#include <unistd.h>

#include "discrete/discrete.hpp"

#define MAX_LINE_SIZE 5000

int*
degree_properties(ogdf::Graph& G);
double*
edge_properties(ogdf::Graph& G, ogdf::GraphAttributes& GA);
// void print_bicon_comp_edge_info(ogdf::EdgeArray<int> &bicon_comp_arr, int
// *bicon_comp_size, int nbicons);
void
derive_CC_subgraphs(ogdf::Graph& G,
                    ogdf::GraphAttributes& GA,
                    ogdf::NodeArray<int>& con_comp_arr,
                    int ncons,
                    ogdf::Graph**& con_subgraphs,
                    ogdf::GraphAttributes**& con_subgraphs_GA);

// Attention, this does not give a NEW string, just a position!
char*
remove_path_from_filename(char* filename)
{
    // Strip path off from filename
    char* fns = strtok(filename, "/");
    char* fn;

    do {
        //        std::cout << fns << std::endl;
        fn = fns;
        fns = strtok(NULL, "/");
    } while (fns != NULL);

    return fn;
}

// level: original graph -> 0, a CC -> 1
void
analyze(ogdf::Graph& G,
        ogdf::GraphAttributes& GA,
        const int level,
        char* const identifier,
        const bool recurseOnCCs,
        const bool csv,
        SubGraph& data)
{

    // index_t n2Cycles = find_all_2_cycles(data).size();
    // double percentage2CycleEdges = ((double)(2*n2Cycles))/((double)data.M());
    // double allEdgesIn2Cycle =  ((2*n2Cycles))/(data.M());

    // index_t reducedGraphEdgeCounter = 0;
    double reducedGraph2CycleEdges = 0;
    double reducedGraph2CycleNodes = 0;

    double reducedGraphNormalEdges = 0;
    double reducedGraphNormalNodes = 0;

    double edgeCounterUndir = 0;
    double nodeCounterUndir = 0;

    double edgeCounterScc = 0;
    double nodeCounterScc = 0;
    // index_t edgeCounterPie = 0;

    const auto [sccsDirected, fvs, sccsUndirected] = fully_reduce_graph(data);
    // const auto sccsDirected = std::get<0>(reductionTuple);
    // const auto sccsUndirected = std::move(std::get<2>(reductionTuple));

    auto nDirected = sccsDirected.size();
    auto nUndirected = sccsUndirected.size();

    for (const auto& compDir : sccsDirected) {
        edgeCounterScc += compDir.M();
        nodeCounterScc += compDir.N();

        // auto reduced = subgraph;
        // auto [reduced, fvs_always_in] = fully_reduce_graph(subgraph);
        // nodeCounterReduction += reduced.N();

        // fully_pie_reduction(reduced);
        // edgeCounterPie += reduced.M();
        // reducedGraphEdgeCounter += reduced.M();
        // reducedGraph2CycleEdges += 2*find_all_2_cycles(reduced).size();
    }

    for (const auto& compUndir : sccsUndirected) {
        edgeCounterUndir += 2 * compUndir.M();
        nodeCounterUndir += compUndir.N();

        // auto reduced = subgraph;
        // auto [reduced, fvs_always_in] = fully_reduce_graph(subgraph);
        // nodeCounterReduction += reduced.N();

        // fully_pie_reduction(reduced);
        // edgeCounterPie += reduced.M();
        // reducedGraphEdgeCounter += reduced.M();
        // reducedGraph2CycleEdges += 2*find_all_2_cycles(reduced).size();
    }
    double nEdgesReduced = ((double)edgeCounterUndir + (double)edgeCounterScc);
    double nNodesReduced = ((double)nodeCounterUndir + (double)nodeCounterScc);

    if (nNodesReduced != 0) {
        reducedGraph2CycleEdges = ((double)edgeCounterUndir) / nEdgesReduced;
        reducedGraph2CycleNodes = ((double)nodeCounterUndir) / nNodesReduced;

        reducedGraphNormalEdges = ((double)edgeCounterScc) / nEdgesReduced;
        reducedGraphNormalNodes = ((double)nodeCounterScc) / nNodesReduced;
    }

    // double reducedEdgesScc =
    // (double)(data.M()-edgeCounterScc)/(double)(data.M()); double
    // reducedNodesReduction =
    // (double)(data.N()-nodeCounterReduction)/(double)(data.N()); double
    // reducedEdgesPie =
    // (double)(edgeCounterScc-edgeCounterPie)/(double)(edgeCounterScc);

    //&double percentage2CycleEdgesReduced =
    //(double)(reducedGraph2CycleEdges)/(double)(reducedGraphEdgeCounter); int
    // allEdgesIn2CycleReduced =
    // (reducedGraph2CycleEdges)/(reducedGraphEdgeCounter);

    GA.directed() = true;

    ogdf::NodeArray<int> connected_comp_arr = ogdf::NodeArray<int>(G);
    long number_of_nodes = G.numberOfNodes();
    long number_of_edges = G.numberOfEdges();
    double density = (double)number_of_edges /
                     (((double)number_of_nodes * (double)number_of_nodes) -
                      (double)number_of_nodes);
    double* const edge_props = edge_properties(G, GA);
    int* const degree_props = degree_properties(G);
    bool planarity = ogdf::isPlanar(G);
    bool bipartite = ogdf::isBipartite(G);
    bool acyclic = ogdf::isAcyclic(G);
    // Not yet clear whether the following works for a single tree as it should.
    //    bool forest = (acyclic ? ogdf::isArborescenceForest(G) : false);
    int num_of_parallel_edges = ogdf::numParallelEdges(G);
    double avg_degree = (2.0 * G.numberOfEdges()) / number_of_nodes;
    int num_connected_components =
      (level ? 1 : strongComponents(G, connected_comp_arr));

    const char* filename = "tracks/properties.json";
    std::ifstream file(filename);
    nlohmann::json json;
    if (!file.fail()) {
        file >> json;
    }
    file.close();
    // TODO overwrite if already exsits
    json["graphs"].push_back(
      { { "id", identifier },
        { "number_of_nodes", number_of_nodes },
        { "number_of_edges", number_of_edges },
        { "density", density },
        { "planarity", planarity },
        { "bipartite", bipartite },
        { "acyclic", acyclic },
        { "min_degree", degree_props[0] },
        { "average_degree", avg_degree },
        { "max_degree", degree_props[1] },
        { "slfl", edge_props[3] },
        { "prll", num_of_parallel_edges },
        { "min_weight", edge_props[0] },
        { "max_weight", edge_props[1] },
        { "no_connected_components", num_connected_components },
        { "nDirected", nDirected },
        { "nUndirected", nUndirected },
        { "2Cycle_edges_density", reducedGraph2CycleEdges },
        { "2Cycle_nodes_density", reducedGraph2CycleNodes },
        { "normal_edges_density", reducedGraphNormalEdges },
        { "normal_nodes_density", reducedGraphNormalNodes },
        { "n_edges_reduced", nEdgesReduced },
        { "n_nodes_reduced", nNodesReduced } });
    std::ofstream o_file(filename);
    o_file << std::setw(4) << json << std::endl;
    o_file.close();

    if (csv) {
        printf("Graph ; |V| ; |A| ; Dsty ; Plnr ; Bprt ; Acyc ; <Dgr ; aDgr ; "
               ">Dgr ; slfl ; prll ; <Wgt ; >Wgt ; nCC \n");
        printf("%8s ; %10ld ; %10ld ; %.4lf ; %d ; %d ; %d ; %d ; %.2lf ; %d ; "
               "%.0lf ; %8d ; %.2lf ; %.2lf ; %d \n",
               identifier,
               number_of_nodes,
               number_of_edges,
               density,
               planarity,
               bipartite,
               acyclic,
               degree_props[0],
               avg_degree,
               degree_props[1],
               edge_props[3],
               num_of_parallel_edges,
               edge_props[0],
               edge_props[1],
               num_connected_components);
    } else {
        printf(
          "Graph \t \t |V| \t \t |A| \t \t Dsty \t \t Plnr \t Bprt \t Acyc \t "
          "<Dgr \t aDgr \t >Dgr \t slfl \t prll \t \t <Wgt \t >Wgt \t nCC  \n");
        printf("%8s \t %10ld \t %10ld \t %.4lf \t %d \t %d \t %d \t %d \t "
               "%.2lf \t %d \t %.0lf \t %8d \t %.2lf \t %.2lf \t %d \n",
               identifier,
               number_of_nodes,
               number_of_edges,
               density,
               planarity,
               bipartite,
               acyclic,
               degree_props[0],
               avg_degree,
               degree_props[1],
               edge_props[3],
               num_of_parallel_edges,
               edge_props[0],
               edge_props[1],
               num_connected_components);
    }

    if (recurseOnCCs && level == 0) {
        ogdf::Graph** con_subgraphs;
        ogdf::GraphAttributes** con_subgraphs_GA;

        derive_CC_subgraphs(G,
                            GA,
                            connected_comp_arr,
                            num_connected_components,
                            con_subgraphs,
                            con_subgraphs_GA);

        for (int i = 0; i < num_connected_components; i++) {
            ogdf::Graph* cc_subgraph = con_subgraphs[i];

            char* comp_index = new char[strlen(identifier) + 20];
            std::sprintf(comp_index, "%s-CC-%d", identifier, i + 1);
            ogdf::GraphAttributes* cc_subgraph_GA = con_subgraphs_GA[i];

            analyze(*cc_subgraph,
                    *cc_subgraph_GA,
                    1,
                    comp_index,
                    recurseOnCCs,
                    csv,
                    data);
            delete[] comp_index;
        }

        delete[] con_subgraphs;
        delete[] con_subgraphs_GA;
    }
}

void
print_usage()
{
    std::cout << "No input file specified. Usage: ./graph-properties "
                 "<filename> [optional: --recursive-cc; --csv]"
              << std::endl;
}

bool
find_option(int argc, char* argv[], const char* option)
{
    for (int i(1); i < argc; ++i) {
        if (0 == strcmp(argv[i], option)) {
            return true;
        }
    }

    return false;
}

int
main(int argc, char* argv[])
{
    if (argc < 2) {
        print_usage();
        return 0;
    }

    char* filename = argv[1];
    Graph data = Graph::read(argv[1]);

    // data.set_reduced_data();

    ogdf::Graph G;
    ogdf::GraphAttributes GA(
      G,
      ogdf::GraphAttributes::edgeDoubleWeight |
        ogdf::GraphAttributes::nodeGraphics |
        ogdf::GraphAttributes::edgeGraphics | ogdf::GraphAttributes::nodeLabel |
        ogdf::GraphAttributes::edgeStyle | ogdf::GraphAttributes::nodeStyle |
        ogdf::GraphAttributes::nodeTemplate);

    ogdf::node* node_array = new ogdf::node[data.N()];

    for (index_t i = 0; i < data.N(); ++i)
        node_array[i] = G.newNode();

    for (index_t e = 0; e < data.M(); ++e) {
        G.newEdge(node_array[data.tails()[e]], node_array[data.heads()[e]]);
    }

    bool recursiveCC(false);

    if (find_option(argc, argv, "--recursive-cc"))
        recursiveCC = true;

    bool csv(false);

    if (find_option(argc, argv, "--csv"))
        csv = true;

    char* fncopy = new char[1 + strlen(filename)];
    strcpy(fncopy, filename);
    SubGraph data_sub(std::move(data));
    analyze(
      G, GA, 0, remove_path_from_filename(fncopy), recursiveCC, csv, data_sub);
    delete[] fncopy;
    exit(0); // currently i dont want graphical output

    // Hierarchical Drawing
    ogdf::SugiyamaLayout SL;
    SL.setRanking(new ogdf::OptimalRanking);
    SL.setCrossMin(new ogdf::MedianHeuristic);

    ogdf::OptimalHierarchyLayout* ohl = new ogdf::OptimalHierarchyLayout;
    ohl->layerDistance(30.0);
    ohl->nodeDistance(25.0);
    ohl->weightBalancing(0.8);
    SL.setLayout(ohl);

    SL.call(GA);

    char* wfn = new char[strlen(argv[1]) + 30];
    sprintf(wfn, "%s-hierarchical.gml", argv[1]);
    ogdf::GraphIO::write(GA, wfn, ogdf::GraphIO::writeGML);
    sprintf(wfn, "%s-hierarchical.svg", argv[1]);
    ogdf::GraphIO::write(GA, wfn, ogdf::GraphIO::drawSVG);

    // Energy-based drawing

    ogdf::FMMMLayout fmmm;

    fmmm.useHighLevelOptions(true);
    fmmm.unitEdgeLength(15.0);
    fmmm.newInitialPlacement(true);
    fmmm.qualityVersusSpeed(
      ogdf::FMMMOptions::QualityVsSpeed::GorgeousAndEfficient);

    fmmm.call(GA);
    sprintf(wfn, "%s-energy.gml", argv[1]);
    ogdf::GraphIO::write(GA, wfn, ogdf::GraphIO::writeGML);
    sprintf(wfn, "%s-energy.svg", argv[1]);
    ogdf::GraphIO::write(GA, wfn, ogdf::GraphIO::drawSVG);

    delete[] wfn;

    delete[] node_array;
    // delete data;

    return 0;
}

/* Iterates through all nodes and finds
 * min degree, max degree, and
 * sum of all degrees (used to calculate avg. degree later).
 * Returns them as an array in that order.
 */
int*
degree_properties(ogdf::Graph& G)
{
    int static results[3];

    if (G.empty()) {
        results[0] = results[1] = 0;
        return results;
    }

    int max_degree = G.chooseNode()->degree();
    int min_degree = max_degree;

    for (ogdf::node n : G.nodes) {
        int current_degree = n->degree();
        if (current_degree > max_degree)
            max_degree = current_degree;
        if (current_degree < min_degree)
            min_degree = current_degree;
    }

    results[0] = min_degree;
    results[1] = max_degree;

    return results;
}

// Prints how many bicons with how many edges are there
// void print_bicon_comp_edge_info(ogdf::EdgeArray<int> &bicon_comp_arr, int
// *bicon_comp_size, int nbicons)
// {
//     if (nbicons < 1)
//         return;

//     if (nbicons == 1)
//     {
//         std::cout << "Biconnected components with " <<
//         std::to_string(bicon_comp_size[0]) << " edges: 1" << std::endl;
//         return;
//     }

//     // Sort to have components of same size next to each other
//     std::sort(bicon_comp_size, bicon_comp_size + nbicons,
//     std::greater<int>());

//     // Iterate over sorted array to print how many of each size exist
//     unsigned int num_of_components = 1;

//     for (int i = 1; i < nbicons; i++)
//     {
//         if (bicon_comp_size[i] == bicon_comp_size[i - 1])
//         {
//             ++num_of_components;

//             if (i == nbicons - 1)
//             {
//                 std::cout << "Biconnected components with " <<
//                 std::to_string(bicon_comp_size[i]) << " edges: " <<
//                 std::to_string(num_of_components) << std::endl;
//             }
//         }
//         else // >
//         {
//             std::cout << "Biconnected components with " <<
//             std::to_string(bicon_comp_size[i - 1]) << " edges: " <<
//             std::to_string(num_of_components) << std::endl; num_of_components
//             = 1;
//         }
//     }
// }

/* Iterates through all edges and finds
 * min weight, max weight, num of zero weights and num of self loops.
 * Returns them as an array in that order.
 */
double*
edge_properties(ogdf::Graph& G, ogdf::GraphAttributes& GA)
{
    double static results[4];

    if (G.numberOfEdges() == 0) {
        results[0] = results[1] = results[2] = results[3] = 0.0;
        return results;
    }

    double min_weight = GA.doubleWeight(G.chooseEdge());
    double max_weight = min_weight;
    int num_of_zero_weights = 0;
    int num_of_self_loops = 0;

    for (ogdf::edge v : G.edges) {
        double edge_weight = GA.doubleWeight(v);
        if (edge_weight < min_weight)
            min_weight = edge_weight;
        if (edge_weight > max_weight)
            max_weight = edge_weight;
        if (edge_weight == 0.0)
            num_of_zero_weights++;
        if (v->isSelfLoop())
            num_of_self_loops++;
    }

    results[0] = min_weight;
    results[1] = max_weight;
    results[2] = num_of_zero_weights;
    results[3] = num_of_self_loops;

    return results;
}

void
derive_CC_subgraphs(ogdf::Graph& G,
                    ogdf::GraphAttributes& GA,
                    ogdf::NodeArray<int>& con_comp_arr,
                    int ncons,
                    ogdf::Graph**& con_subgraphs,
                    ogdf::GraphAttributes**& con_subgraphs_GA)
{
    con_subgraphs = new ogdf::Graph*[ncons];
    con_subgraphs_GA = new ogdf::GraphAttributes*[ncons];

    for (int i = 0; i < ncons; i++) {
        ogdf::Graph* N = new ogdf::Graph();
        con_subgraphs[i] = N;
        con_subgraphs_GA[i] = new ogdf::GraphAttributes(
          *N, ogdf::GraphAttributes::edgeDoubleWeight);
    }

    std::map<ogdf::node, ogdf::node>* con_node_map =
      new std::map<ogdf::node, ogdf::node>[ncons];

    for (ogdf::edge e : G.edges) {
        int CCs = con_comp_arr[e->source()];

        std::map<ogdf::node, ogdf::node>::iterator b_it =
          con_node_map[CCs].find(e->source());

        if (b_it == con_node_map[CCs].end()) {
            ogdf::node n = con_subgraphs[CCs]->newNode();
            con_node_map[CCs][e->source()] = n;
        }

        int CCt = con_comp_arr[e->target()];

        b_it = con_node_map[CCt].find(e->target());
        if (b_it == con_node_map[CCt].end()) {
            ogdf::node n = con_subgraphs[CCt]->newNode();
            con_node_map[CCt][e->target()] = n;
        }

        if (CCs != CCt) {
            std::cerr
              << "Edge Endpoints not in the same CC - this should never happen!"
              << std::endl;
        }

        ogdf::edge f = con_subgraphs[CCs]->newEdge(
          con_node_map[CCs][e->source()], con_node_map[CCs][e->target()]);
        con_subgraphs_GA[CCs]->doubleWeight(f) = GA.doubleWeight(e);
    }

    // Handle isolated nodes!
    for (ogdf::node v : G.nodes) {
        int CC = con_comp_arr[v];

        std::map<ogdf::node, ogdf::node>::iterator b_it =
          con_node_map[CC].find(v);

        if (b_it == con_node_map[CC].end()) {
            ogdf::node n = con_subgraphs[CC]->newNode();
            con_node_map[CC][v] = n;
        }
    }

    delete[] con_node_map;
}
