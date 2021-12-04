#include "isomorph.hpp"
#include "view.hpp"
#include "utility.hpp"

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
    bool unstable = false;
    do
	res = classify(G);
    while(recolor(res, G));
    return res;
}

permutation solver(Graph& G, Graph& H, AfterStable mode, morph_iter_callback_t callback)
{
    permutation result;
    classification classes_G, classes_H;
    bool unstable = false, g_unstable, h_unstable;
    do {
        classes_G = classify(G);
        g_unstable = recolor(classes_G, G);

        classes_H = classify(H);
        h_unstable = recolor(classes_H, H);

        if (callback) callback();

        if (classes_G.size() != classes_H.size() || g_unstable != h_unstable){
            return result;
        }

        unstable = g_unstable && h_unstable;

        if (!unstable){
            switch (mode) {
            case AfterStable::None:
                break;
            case AfterStable::Bruteforce:
                break;
            case AfterStable::Destabilize:
                break;
            default:
                break;
            }
            if (mode == AfterStable::Bruteforce){
                brute_force_classified(classes_G, classes_H, G, H);
            } else {
                g_unstable = destabilize(classes_G, G);
                h_unstable = destabilize(classes_H, H);
                if (classes_G.size() != classes_H.size() || g_unstable != h_unstable){
                    return result;
                }

                unstable = g_unstable && h_unstable;
            }
        }
    } while(unstable);
    
    return result;
}
