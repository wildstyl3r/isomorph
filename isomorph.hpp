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

class ClassificationReport{
    Graph& G;
    classification _classes;
    std::chrono::high_resolution_clock::duration _time;
    value _iterations;
    vector<View*> _views;
public:
    ClassificationReport(Graph& g);
    ~ClassificationReport();
    const classification& classes();
    const std::chrono::high_resolution_clock::duration& time();
    size_t iterations();
    const vector<View*>& views();
    void recalc();
};

class Isomorph{
    ClassificationReport _g;
    ClassificationReport _h;
    permutation _perm;
public:
    Isomorph(Graph& g, Graph& h, AfterStable mode);
    permutation& perm();
    ClassificationReport& g();
    ClassificationReport& h();
};

vector<vector<edge>> deconstruct(Graph& G);


#endif
