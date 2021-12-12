#include "view.hpp"
#include "ordering.hpp"

View::View(Graph &g, vertex root) :
    tree(nullptr),
	_src (g),
	_root(root),
	_distance(g.V().size(), g.V().size()+1),
	_inlet_degree(g.V().size(), 0),
	_height(0)
{
    queue<vertex> bfs;

    _distance[_root] = 0;
    _height = 1;

    //step 1: distance
    vector<bool> used(_src.V().size(), false);
    used[_root] = true;
    bfs.push(_root);
    while(!bfs.empty()){
        vertex v = bfs.front();
        bfs.pop();
        for(auto u : _src.V(v)){
            if (used[u] == false){
                used[u] = true;
                _distance[u] = _distance[v] + 1;
                _height = _distance[u]+1;
                bfs.push(u);
            }
        }
    }

    _dockyard.resize(_height);
    _adjacency_vector.resize(_src.V().size());

    for(vertex v = 0; v < _src.V().size(); ++v){
        _dockyard[_distance[v]].push_back(v);

        //step 2: edges oriented by distance growth
        for(vertex u = 0; u < _src.V().size(); ++u){
            if (_src.has({v,u}) && _distance[v] < _distance[u]){
                _adjacency_vector[v].insert(u);
                _inlet_degree[u]++;
            }
        }
    }
}


void View::mainline(vertex start,   //фактически, идеально информированный поиск в глубину с предподсчетом перед каждым заглублением
            vector< LexMSTNode* >& nodes) //TODO: переписать в виде цикла со стеком в lexmst(), проверить возможности ускорения предподсчета
{
    VxOrder ord;
    for(vertex c : V(start)){
        for(value l = _dockyard.size()-1; l > 0; l--){
            for(auto i = _dockyard[l].begin(); i != _dockyard[l].end(); i++){
                for(auto j = i; j != _dockyard[l].end(); j++){
                    if (i == j || nodes[*i]->used || nodes[*j]->used) continue;

                    vector<vertex> i_childset, j_childset;
                    if (l < _dockyard.size()-1) {
                        // at this moment dockyard[l+1] is already sorted and used vertices are in the tail
                        for (auto k = _dockyard[l+1].begin(); k != _dockyard[l+1].end() &&
                             !nodes[*k]->used; k++) {

                            if (V(*i).count(*k)) {
                                i_childset.push_back(*k);
                            }
                            if (V(*j).count(*k)) {
                                j_childset.push_back(*k);
                            }
                        }
                    }

                    int comp = ord.childset_compare(i_childset, j_childset); //check depth, outlet degree

                    if (comp == 0) { //check inlet degree
                        value i_up_reject = 0, j_up_reject = 0;
                        if(l > 1){
                            for (auto k = _dockyard[l-1].begin(); k != _dockyard[l-1].end(); k++) {
                                if (V(*k).count(*i) && nodes[*k]->used) {
                                    i_up_reject++;
                                }
                                if (V(*k).count(*j) && nodes[*k]->used) {
                                    j_up_reject++;
                                }
                            }
                        }
                        value i_actual_in_deg = in_deg(*i) - i_up_reject;
                        value j_actual_in_deg = in_deg(*j) - j_up_reject;
                        comp = (i_actual_in_deg == j_actual_in_deg ? 0 : (i_actual_in_deg < j_actual_in_deg ? -1 : +1));
                    }

                    // check color
                    if (comp == 0) {
                        comp = (color(*i) == color(*j) ? 0 : (color(*i) < color(*j) ? -1 : +1));
                    }
                    ord.set(*i, *j, comp);
                }
            }
            _dockyard[l].sort([&](vertex lhs, vertex rhs){
                if (nodes[lhs]->used) return false;
                if (nodes[rhs]->used) return true;
                return ord.get(lhs, rhs) == -1;
            });
        }

        //every level in the dockyard is sorted at this moment
        nodes[start]->used = true;
        value start_children = _distance[start] + 1;
        vertex v = start;
        stack<vertex> branch;
        for(value level = start_children; level < _dockyard.size(); ++level){
            for(vertex u : _dockyard[level]){ //вершина, достижимая из предыдущей может быть и не первой в списке
                if (!nodes[u]->used && this->has({v,u})) {
                    nodes[u]->parent = nodes[v];
                    nodes[u]->used = true;

                    nodes[v]->children.push_back(nodes[u]);
                    branch.push(u);
                    _lexmst.push_back({v,u});
                    v = u;
                    break;
                }
            }
        }

        while(!branch.empty()){
            mainline(branch.top(), nodes);
            branch.pop();
        }
    }
}


streeng View::lexmst_string()
{
    if (_lexmst_streeng.length() == 0) {
        lexmst();
    }
    return _lexmst_streeng;
}

vector<edge> View::lexmst_edges()
{
    if (_lexmst.size() == 0) {
        lexmst();
    }
    return _lexmst;
}

void View::lexmst()
{
    delete tree;
    _lexmst.clear();
    vector< LexMSTNode* > nodes(V().size(), nullptr);
    for(vertex i = 0; i < V().size(); i++){
        nodes[i] =  new LexMSTNode(in_deg(i), out_deg(i), color(i));
    }
    
    mainline(root(), nodes);
    
    _lexmst_streeng = nodes[root()]->get_str();
    tree = new Graph(_lexmst);
    for(vertex v = 0; v < V().size(); ++v){
        tree->set_color(v, color(v));
    }
    
    for(vertex i = 0; i < V().size(); i++){
        delete nodes[i];
    }
}

Graph* View::lexmst_tree()
{
    if (tree == nullptr) {
        lexmst();
    }
    return tree;
}

View::~View(){
    if (tree != nullptr) {
        delete tree;
    }
}

value View::color(vertex v) const {
    return _src.color(v);
}

string View::label(vertex v) const {
    return _src.label(v);
}

string View::id(vertex v) const {
    return _src.id(v);
}

size_t View::count_colors() const {
    return _src.count_colors();
}
