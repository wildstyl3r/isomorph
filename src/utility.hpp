#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "isomorph.hpp"

/* recolor returns true if given coloring has more colors than
 * previous (previous is unstable)
 */
bool recolor(const classification& classes, Graph* G);


/* make_vector returns vector of parts for use in combinatorics (brute
 * mode)
 */
vector< vector<vertex> > make_vector(const classification& classes);


/* concat unites parts into permutation
 */
permutation concat(const vector< vector<vertex> >& classes);


/* check_naive performs comparison of two graphs for given permutations
 */
bool check_naive(permutation& g_p, permutation& h_p, Graph& G, Graph& H);


/* get_permutation takes substitution and returns normalized
   permutation for H
 */
permutation get_permutation(permutation& g_p, permutation& h_p);

/* destabilize returns true if given coloring has more colors than
 * previous (previous is unstable)
 */
bool destabilize(const classification& classes, Graph& G);

classification classify_with_views(Graph* G, vector<View*>& views, std::function<void(bool)> callback = nullptr);

permutation brute_force_classified(const classification& classes_G, const classification& classes_H, Graph& G, Graph& H);

#endif
