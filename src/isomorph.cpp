#include "isomorph.hpp"
#include "utility.hpp"

using classification_with_views = std::pair<classification, vector<View*>>;

classification classify(Graph& G)
{
    classification res;
    for(vertex i = 0; i < G.V().size(); i++){
        View view(G, i);
        streeng s = view.lexmst_string();
        res[s].push_back(i);
    }
    return res;
}

vector<vector<edge>> deconstruct(Graph& G)
{
    vector<vector<edge>> res;
    for(vertex i = 0; i < G.V().size(); i++){
        res.push_back(View(G, i).lexmst_edges());
    }
    return res;
}

classification classify_canonical(Graph& G)
{
    classification res;
    size_t iterations = 0;
    do{
        res = classify(G);
        iterations++;
    }
    while(recolor(res, G));
    return res;
}

IsomorphReport solver(Graph& G, Graph& H, AfterStable mode, morph_iter_callback_t callback)
{
    IsomorphReport result;
    for(vertex v = 0; v < G.V().size(); ++v){
        result.g.views.push_back(new View(G, v));
    }
    for(vertex v = 0; v < H.V().size(); ++v){
        result.h.views.push_back(new View(H, v));
    }
    bool unstable = false, g_unstable, h_unstable;
    do {
        result.g.classes = classify_with_views(G, result.g.views);
        g_unstable = recolor(result.g.classes, G);

        result.h.classes = classify_with_views(H, result.h.views);
        h_unstable = recolor(result.h.classes, H);

        if (callback) callback();

        if (result.g.classes.size() != result.h.classes.size() || g_unstable != h_unstable){
            return result;
        }

        unstable = g_unstable && h_unstable;

        if (!unstable){
            switch (mode) {
            case AfterStable::None:
                break;
            case AfterStable::Bruteforce:
                result.perm = brute_force_classified(result.g.classes, result.h.classes, G, H);
                return result;
                break;
            case AfterStable::Destabilize:
                g_unstable = destabilize(result.g.classes, G);
                h_unstable = destabilize(result.h.classes, H);
                if (result.g.classes.size() != result.h.classes.size() || g_unstable != h_unstable){
                    return result;
                }

                unstable = g_unstable && h_unstable;
                break;
            default:
                break;
            }
        }
    } while(unstable);

    for(auto eq_class : result.g.classes){
       if(result.h.classes[eq_class.first] != eq_class.second){
           return result;
       }
    }
    auto cv_g = concat(make_vector(result.g.classes)), cv_h = concat(make_vector(result.h.classes));
    result.perm = get_permutation(cv_g, cv_h);
    
    return result;
}

ClassificationReport classify_canonical_ext(Graph& G)
{
    ClassificationReport res;
    res.iterations = 0;
    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    for(vertex v = 0; v < G.V().size(); ++v){
        res.views.push_back(new View(G, v));
    }
    do{
        res.classes = classify_with_views(G, res.views);
        res.iterations++;
    }
    while(recolor(res.classes, G));
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    res.time = (end-begin);
    return res;
}

classification classify_with_views(Graph& G, vector<View*>& views)
{
    classification res;
    for(vertex i = 0; i < G.V().size(); i++){
        views[i]->lexmst();
        streeng s = views[i]->lexmst_string();
        res[s].push_back(i);
    }
    return res;
}
