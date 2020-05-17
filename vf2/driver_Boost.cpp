#include "../GraphDS.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/vf2_sub_graph_iso.hpp>

using namespace boost;

template <typename Graph1, typename Graph2>
struct vf2_callback
{
    template <typename CorrespondenceMap1To2, typename CorrespondenceMap2To1>
    bool operator()(CorrespondenceMap1To2 f, CorrespondenceMap2To1) const
    {
        return true;
    }
};

bool solve(TGraph::Graph &P, TGraph::Graph &G)
{
    typedef property<edge_name_t, int> edge_property;
    typedef property<vertex_name_t, int, property<vertex_index_t, int>>
        vertex_property;
    typedef adjacency_list<vecS, vecS, undirectedS, vertex_property,
                           edge_property>
        graph_type;
    graph_type pp, gg;
    for (int i = 0; i < P.n; i++)
        add_vertex(vertex_property(P.label[i + 1]), pp);
    for (int i = 0; i < P.n; i++)
        for (int j = i + 1; j < P.n; j++)
            if (P.g[i + 1][j + 1])
                add_edge(i, j, edge_property(0), pp);
    for (int i = 0; i < G.n; i++)
        add_vertex(vertex_property(G.label[i + 1]), gg);
    for (int i = 0; i < G.n; i++)
        for (int j = i + 1; j < G.n; j++)
            if (G.g[i + 1][j + 1])
                add_edge(i, j, edge_property(0), gg);

    typedef property_map<graph_type, vertex_name_t>::type vertex_name_map_t;
    typedef property_map_equivalent<vertex_name_map_t, vertex_name_map_t>
        vertex_comp_t;
    vertex_comp_t vertex_comp = make_property_map_equivalent(
        get(vertex_name, pp), get(vertex_name, gg));

    typedef property_map<graph_type, edge_name_t>::type edge_name_map_t;
    typedef property_map_equivalent<edge_name_map_t, edge_name_map_t>
        edge_comp_t;
    edge_comp_t edge_comp = make_property_map_equivalent(
        get(edge_name, pp), get(edge_name, gg));

    vf2_callback<graph_type, graph_type> callback;

    return vf2_subgraph_iso(pp, gg, callback, vertex_order_by_mult(pp),
                            edges_equivalent(edge_comp).vertices_equivalent(vertex_comp));
}