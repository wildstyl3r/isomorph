#include "utility.hpp"

bool recolor(const classification& classes, Graph* G)
{
    // classify() did not change coloring, only previous recolor() could
    if (G->count_colors() == classes.size()){
        return false;
    }

    value color = 0;
    for(const auto& eq_class : classes){
        for(const vertex& v : eq_class.second) {
            G->set_color(v, color);
        }
        color++;
    }
    return true;
}

vector< vector<vertex> > make_vector(const classification& classes)
{
    vector< vector<vertex> > res;
    for (const auto& kv : classes){
        res.push_back(kv.second);
    }
    return res;
}

permutation concat(const vector< vector<vertex> >& classes)
{
    permutation res;
    for (const auto& v : classes){
        res.insert(res.end(), v.begin(), v.end());
    }
    return res;
}

bool check_naive(permutation& g_p, permutation& h_p, Graph& G, Graph& H)
{
    for(size_t i = 0; i < G.V().size(); ++i)
	for(size_t j = 0; j < G.V().size(); ++j)
	    if (G(g_p[i]).count(g_p[j]) != H(h_p[i]).count(h_p[j])) {
		return false;
	    }
    return true;
}

permutation get_permutation(permutation& g_p, permutation& h_p)
{
    vector<std::pair<vertex, vertex>> change(g_p.size());
    for (int i = 0; i < g_p.size(); ++i){
        change[i] = {g_p[i], h_p[i]};
    }
    std::sort(change.begin(), change.end());

    permutation perm(h_p.size());
    for (int i = 0; i < h_p.size(); ++i){
        perm[i] = change[i].second;
    }
    return perm;
}

bool destabilize(const classification& classes, Graph& G)
{
    for (const auto& c : classes){
        if (c.second.size() > 1){
            G.set_color(c.second.front(), classes.size());
            return true;
        }
    }
    return false;
}

permutation brute_force_classified(const classification& classes_G, const classification& classes_H, Graph& G, Graph& H)
{
    permutation g_perm = concat(make_vector(classes_G));
    vector<vector<vertex>> parts_h_perm = make_vector(classes_H);

    bool enough = false;		
    auto it = parts_h_perm.begin();
    while(!enough){
	permutation h_perm = concat(parts_h_perm);
	if (check_naive(g_perm, h_perm, G, H)) {
	    return get_permutation(g_perm, h_perm);
	}
	while(!next_permutation(it->begin(), it->end())){
	    it ++;
	    if (it == parts_h_perm.end()) {
		enough = true;
		break;
	    }
	}
    }
    return permutation();
}

classification classify_with_views(Graph* G, vector<View*>& views,
                                   std::function<void(bool)> callback)
{
    classification res;
    for(vertex i = 0; i < G->V().size(); i++){
        views[i]->lexmst();
        streeng s = views[i]->lexmst_string();
        res[s].push_back(i);
        if (callback) callback(false);
    }
    return res;
}
