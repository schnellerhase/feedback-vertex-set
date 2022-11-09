// #ifndef GUARD_DOUBLE_DEP_HPP
// #define GUARD_DOUBLE_DEP_HPP

// #include "scc.hpp"
// #include "../graph.hpp"
// #include "../subgraph.hpp"
// #include "reduction/edge.hpp"

// // #include "basic_graph_reductions.hpp"

// void remove_edges_list(SubGraph &subgraph, std::vector<bool> &edgesToRemove)
// {
//     index_t edge_to_remove = edgesToRemove.size();
//     while (edge_to_remove > 0)
//     {
//         edge_to_remove--;
//         if (edgesToRemove[edge_to_remove]) remove_edge(subgraph,
//         edge_to_remove);
//     }
//     return;
// }

// std::vector<std::tuple<index_t, index_t>> find_all_2_cycles(const SubGraph
// &subgraph)
// {
//     std::vector<std::tuple<index_t, index_t>> twoCycles;
//     for (index_t node(0); node < subgraph.N(); node++)
//     {
//         for (index_t outneighbor(0); outneighbor <
//         subgraph.outadj()[node].size(); outneighbor++)
//         {
//             if (subgraph.outadj()[node][outneighbor] > node)
//             {
//                 for (index_t inneighbor(0); inneighbor <
//                 subgraph.inadj()[node].size(); inneighbor++)
//                 {
//                     if (subgraph.outadj()[node][outneighbor] ==
//                     subgraph.inadj()[node][inneighbor] )
//                     {
//                         twoCycles.push_back(std::tuple<index_t,index_t>{node,
//                         subgraph.outadj()[node][outneighbor]});
//                     }
//                 }
//             }
//         }
//     }
//     return twoCycles;
// }

// // void fully_double_reduction(SubGraph &subgraph) // TO DO!!!
// // {
// //     std::vector<std::tuple<index_t, index_t>> twoCycles =
// find_all_2_cycles(subgraph);
// //     std::vector<bool> edgesToRemove(subgraph.M(), false);
// //     std::cout << "double reducing..." << std::endl;
// //     for (index_t cycleIndex(0); cycleIndex < twoCycles.size();
// cycleIndex++)
// //     {
// //         double_reduction( edgesToRemove, subgraph,
// std::get<0>(twoCycles[cycleIndex]), std::get<1>(twoCycles[cycleIndex]));
// //     }
// //     std::cout << "removing "<< edgesToRemove.size() << " edges!" <<
// std::endl;
// //     remove_edges_list(subgraph, edgesToRemove);
// //     return;
// // }

// std::vector<std::tuple<index_t, index_t>> pie_reduction(SubGraph &subgraph)
// {
//     std::vector<std::tuple<index_t, index_t>> tailsHeadsToRemove;
// IndexList components = scc_pie_version(subgraph);

// for (index_t edge(0); edge < subgraph.M(); edge++)
// {
//     index_t head = subgraph.heads()[edge];
//     index_t tail = subgraph.tails()[edge];
//     if (components[head] != components[tail])
//     {
//         tailsHeadsToRemove.push_back(std::tuple<index_t,index_t>{tail,
//         head});
//     }
// }
// return tailsHeadsToRemove;
// }

// // IndexList find_all_2_cycle_edges(const SubGraph &subgraph)
// // {
// //     IndexList twoCycleEdges;
// //     for (index_t edge(0); edge < subgraph.M(); edge++)
// //     {
// //         index_t tail = subgraph.tails()[edge];
// //         index_t head = subgraph.heads()[edge];
// //         for (index_t outneighbor(0); outneighbor <
// subgraph.outadj()[head].size(); outneighbor++)
// //         {
// //             if (subgraph.outadj()[head][outneighbor] == tail)
// //             {
// //                 twoCycleEdges.push_back(edge);
// //             }
// //         }
// //     }
// //     return twoCycleEdges;
// // }

// void remove_edges_tails_heads_orderd(SubGraph &subgraph,
// std::vector<std::tuple<index_t, index_t>> &tailsHeadsToRemove)
// {
//     index_t edgeToRemoveItterator = tailsHeadsToRemove.size();
//     index_t edgeItterator = subgraph.M();
//     if (edgeToRemoveItterator == 0) return;

//     index_t tempTail = std::get<0>(tailsHeadsToRemove[edgeToRemoveItterator -
//     1]); index_t tempHead =
//     std::get<1>(tailsHeadsToRemove[edgeToRemoveItterator - 1]); while
//     (edgeToRemoveItterator > 0)
//     {
//         edgeItterator--;
//         if ((tempTail == subgraph.tails()[edgeItterator]) && (tempHead ==
//         subgraph.heads()[edgeItterator]))
//         {
//             remove_edge(subgraph, edgeItterator);
//             edgeToRemoveItterator--;
//             if (edgeToRemoveItterator > 0)
//             {
//                 tempTail =
//                 std::get<0>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
//                 tempHead =
//                 std::get<1>(tailsHeadsToRemove[edgeToRemoveItterator - 1]);
//             }
//         }
//     }
//     return;
// }

// void fully_pie_reduction(SubGraph &graph) // TO DO!!!
// {
//     SubGraph subgraph(graph);
//     IndexList twoCycleEdges = find_all_2_cycle_edges(subgraph);
//     index_t edgeItterator = twoCycleEdges.size();

//     while(edgeItterator > 0)
//     {
//         edgeItterator--;
//         remove_edge(subgraph, twoCycleEdges[edgeItterator]);
//     }

//     std::vector<std::tuple<index_t, index_t>> tailsHeadsToRemove =
//     pie_reduction(subgraph); remove_edges_tails_heads_orderd(graph,
//     tailsHeadsToRemove); return;
// }

// #endif //GUARD_DOUBLE_DEP_HPP