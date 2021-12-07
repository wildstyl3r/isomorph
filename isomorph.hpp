#ifndef ISOMORPH_HPP
#define ISOMORPH_HPP

#include "common.hpp"
#include "view.hpp"


/* solver searches for isomorphism G -> H, brute_mode determines
 * behaviour after coloring stabilization:
 * if brute mode is true, then it uses brute force to find out
 * permutation, else it destabilizes coloring by splitting first class
 * that has more than one element and return to cycle, doing this
 * until number of classes isn't equal to number of vertices
 */

struct ClassificationReport{
    classification classes;
    std::chrono::high_resolution_clock::duration time;
    value iterations;
    vector<View*> views;
};

struct IsomorphReport{
    ClassificationReport g;
    ClassificationReport h;
    permutation perm;
};

IsomorphReport solver(Graph& G, Graph& H, AfterStable mode = AfterStable::Destabilize, morph_iter_callback_t callback = nullptr);

classification classify(Graph& G);

classification classify_canonical(Graph& G);

vector<vector<edge>> deconstruct(Graph& G);


ClassificationReport classify_canonical_ext(Graph& G);
ClassificationReport classify_canonical_ext_old(Graph& G);
classification classify_with_views(Graph& G, vector<View*>& views);

#endif
