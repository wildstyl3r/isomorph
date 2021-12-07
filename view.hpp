#ifndef VIEW_HPP
#define VIEW_HPP
#include "common.hpp"
#include "view.hpp"

using std::queue;
using std::map;
using std::stack;


class View : public Graph
{
    Graph& _src;
    Graph* tree;
    const vertex _root;
    attributes _distance;
    attributes _inlet_degree;
    value _height;
    streeng _lexmst_streeng;
    vector<edge> _lexmst;
    vector< std::list<vertex> > _dockyard;

    void mainline(vertex start, std::vector< LexMSTNode* >& nodes);
    
public:
    View(Graph &g, vertex root);
    ~View();

    vertex root() { return _root; }

    value dist(vertex v) { return _distance[v]; }
    value height() { return _height; }

    value in_deg(vertex v) { return _inlet_degree[v]; }
    value out_deg(vertex v) { return _adjacency_vector[v].size(); }
    value color(vertex v);

    string label(vertex v);
    string id(vertex v);
    size_t count_colors();

    streeng lexmst_string();
    vector<edge> lexmst_edges();
    void lexmst();
    Graph* lexmst_tree();
};
#endif
