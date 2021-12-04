#include "view.hpp"
#include "ordering.hpp"

View::View(Graph &g, vertex root) : 
	Graph(g.degrees()),
	_src (g),
	_root(root),
	_distance(g.V().size(), g.V().size()+1),
	_inlet_degree(g.V().size(), 0),
	_height(0)
	{   
	    const unsigned long inf = _src.V().size()+1;
	    
	    queue<vertex> bfs;

	    _distance[_root] = 0;

	    //step 1: distance
	    vector<bool> used(_src.V().size(), false);
	    used[_root] = true;
	    bfs.push(_root);
	    while(!bfs.empty()){
		vertex v = bfs.front();
		bfs.pop();
		for(auto u : _src.V()[v]){
		    if (used[u] == false){
			used[u] = true;
			_distance[u] = _distance[v] + 1;
			_height = _distance[u];
			bfs.push(u);
		    }
		}
	    }
	    
	    //step 2: edges oriented by distance growth
	    bfs.push(root);
	    while(!bfs.empty()){
		vertex v = bfs.front();
		bfs.pop();
		for(auto u : _src.V()[v]){
		    if (_distance[v] < _distance[u]){
			_adjacency_vector[v].insert(u);
			_inlet_degree[u]++;
			bfs.push(u);
		    }
		}
	    }
	}


void View::mainline(vertex start,
		    unordered_set<vertex>& workset,
		    vector< LexMSTNode* >& nodes)
{
    if (workset.empty()) return;

    VxOrder ord;
    vector< std::list<vertex> > dockyard(height() - dist(start));
    for(const vertex& v : workset){
	dockyard[dist(v) - dist(start)].push_back(v);
    }

    for(const vertex& v : dockyard.back())
	for(const vertex& u : dockyard.back()){
	    if (v != u) {
		ord.set(v, u, color(v) == color(u) ? 0 : (color(v) < color(u) ? -1 : 1) );
	    }
	}

    for(value l = dockyard.size()-1; l > 0; l--){
	for(auto i = dockyard[l].begin(); i != dockyard[l].end(); i++){
	    for(auto j = i; j != dockyard[l].end(); j++){
		if (i == j) continue;

		vector<vertex> i_childset, j_childset;
		if (l != dockyard.size()-1) {
		    // at this moment dockyard[l+1] is already sorted
		    for(auto k = dockyard[l+1].begin(); k != dockyard[l+1].end(); k++){
			if (V()[*i].count(*k)) {
			    i_childset.push_back(*k);
			}
			if (V()[*j].count(*k)) {
			    j_childset.push_back(*k);
			}
		    }
		}

		int comp = ord.childset_compare(i_childset, j_childset); // depth, outlet degree
		if (comp == 0) { //inlet degree
		    comp = (in_deg(*i) == in_deg(*j) ? 0 : (in_deg(*i) < in_deg(*j) ? -1 : +1));
		}

		// color check
		if (comp == 0) {
		    comp = (color(*i) == color(*j) ? 0 : (color(*i) < color(*j) ? -1 : +1));
		}
		ord.set(*i, *j, comp);
	    }
	}
	dockyard[l].sort([&](vertex lhs, vertex rhs){
	    return ord.get(lhs, rhs) == -1;
	});
    }

    //every level in the dockyard is sorted at this moment
    vertex v = start;
    stack<vertex> branches;
    for(value level = 1; level < dockyard.size(); ++level){
	for(vertex u : dockyard[level]){
	    if (V()[v].count(u)) {
		nodes[v]->children.push_back(nodes[u]);
		nodes[u]->parent = nodes[v];
		nodes[u]->used = true;
		branches.push(u);
		_lexmst.push_back({v,u});
		v = u;
		break;
	    }
	}
    }

    dockyard.clear();

    while(!branches.empty()){
	unordered_set<vertex> ws;
		
	queue<vertex> bfs;
	bfs.push(branches.top());
	while(!bfs.empty()){
	    vertex v = bfs.front();
	    bfs.pop();
	    for(vertex u : V()[v]){
		if (!nodes[u]->used) {
		    ws.insert(u);
		    bfs.push(u);
		}
	    }
	}
	mainline(branches.top(), ws, nodes);
	branches.pop();
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
    vector< LexMSTNode* > nodes(V().size(), nullptr);
    unordered_set<vertex> initial_ws;
    for(vertex i = 0; i < V().size(); i++){
	nodes[i] =  new LexMSTNode(in_deg(i), out_deg(i), color(i));
	initial_ws.insert(i);
    }
    nodes[root()]->used = true;

    mainline(root(), initial_ws, nodes);

    _lexmst_streeng = nodes[root()]->getStr();
    
    for(vertex i = 0; i < V().size(); i++){
	delete nodes[i];
    }
}
