import json
import matplotlib.pyplot as plt

with open('properties.json') as f:
    data = json.load(f)

graphs = data['graphs']

# Possible properties:
# "acyclic" = FALSE (CONST), if it was acyclic -> trivial solution
# "average_degree"
# "bipartite"
# "density"
# "id"
# "max_degree"
# "max_weight" = 1 (CONST)
# "min_degree"
# "min_weight" = 1 (CONST)
# "no_connected_components"
# "number_of_edges"
# "number_of_nodes"
# "planarity"
# "prll"
# "slfl"

def plot_property(prop, prop2 = None, log=False):
    x = []
    y = []
    for graph in graphs:
        x += [graph['id']]
        y += [graph[prop]]
    
    plt.plot(x, y, marker='x', linestyle='None', label = prop)

    if prop2 is not None:
        x = []
        y2 = []
        for graph in graphs:
            x += [graph['id']]
            y2 += [graph[prop2]]

        plt.plot(x, y2, marker='x', linestyle='None', label = prop2)
    plt.xticks(rotation=90)
    if log:
        plt.yscale('log')
    #plt.ylabel(prop)
    # plt.xlabel('graph')
    #xticks = [i+1 for i in range(0, 200, 2)]
    #xlabels = [f"e_{str(t).zfill(3)}" for t in xticks]
    #plt.xticks(xticks, xlabels, rotation=90)
    #plt.xlim((0,200))
    plt.legend()
    plt.gcf().set_size_inches(20,5)
    plt.tight_layout()

    plt.savefig(f'{prop}.png', dpi=200)
    plt.clf()
    # plt.show()

# plot_property('reducedNodesReduction')
# plot_property('reducedEdgesScc')
# plot_property('reducedEdgesPie')
plot_property('2Cycle_nodes_density','normal_nodes_density')
plot_property('2Cycle_edges_density','normal_edges_density')
plot_property('acyclic')
plot_property('average_degree')
plot_property('bipartite')
plot_property('density')
plot_property('max_degree')
plot_property('min_degree')
plot_property('no_connected_components', log=True)
plot_property('number_of_edges','n_edges_reduced', log=True)
plot_property('number_of_nodes','n_nodes_reduced', log=True)
plot_property('planarity')
plot_property('prll')
plot_property('slfl')
