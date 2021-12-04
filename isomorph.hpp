#ifndef ISOMORPH_HPP
#define ISOMORPH_HPP

#include <functional>
#include <map>
#include "grypho.hpp"

using morph_iter_callback_t = std::function<void()>;
using streeng_symbol        = value;
using streeng               = std::basic_string<streeng_symbol>;
using classification        = std::map< streeng, vector<vertex> >;

enum class AfterStable {
    None,
    Bruteforce,
    Destabilize,
};

/* solver searches for isomorphism G -> H, brute_mode determines
 * behaviour after coloring stabilization:
 * if brute mode is true, then it uses brute force to find out
 * permutation, else it destabilizes coloring by splitting first class
 * that has more than one element and return to cycle, doing this
 * until number of classes isn't equal to number of vertices
 */
permutation solver(Graph& G, Graph& H, AfterStable mode = AfterStable::Destabilize, morph_iter_callback_t callback = nullptr);

classification classify(Graph& G);

classification classify_canonical(Graph& G);

vector<vector<edge>> deconstruct(Graph& G);


#endif
