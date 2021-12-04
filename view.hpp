#ifndef VIEW_HPP
#define VIEW_HPP
#include "grypho.hpp"
#include "lex_mst_node.hpp"
#include <queue>
#include <stack>
#include <list>
#include <algorithm>

using std::queue;
using std::map;
using std::stack;


class View : public Graph
{
    Graph& _src;
    const vertex _root;
    attributes _distance;
    attributes _inlet_degree;
    value _height;
    streeng _lexmst_streeng;
    vector<edge> _lexmst;

    void mainline(vertex start, std::unordered_set<vertex>& workset, std::vector< LexMSTNode* >& nodes);
    void lexmst();
    
public:
    View(Graph &g, vertex root);

    vertex root() { return _root; }

    value dist(vertex v) { return _distance[v]; }
    value height() { return _height; }

    value in_deg(vertex v) { return _inlet_degree[v]; }
    value out_deg(vertex v) { return _adjacency_vector[v].size(); }

    streeng lexmst_string();
    vector<edge> lexmst_edges();
};
#endif
