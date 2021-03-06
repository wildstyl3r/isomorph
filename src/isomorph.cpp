#include "isomorph.hpp"
#include "utility.hpp"

using classification_with_views = std::pair<classification, vector<View*>>;

ClassificationReport::ClassificationReport(Graph& g, std::function<void(bool)> callback) : G(&g), _iterations(0) {
    for(vertex v = 0; v < g.V().size(); ++v){
        _views.push_back(new View(g, v));
        if(callback) callback(false);
    }

    std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
    do{
        _classes = classify_with_views(G, _views, callback);
        _iterations++;
    }
    while(recolor(_classes, G));
    if(callback) callback(true);
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    _time = (end-begin);
}

ClassificationReport::~ClassificationReport(){
    for(View* view : _views){
        delete view;
    }
}

const classification& ClassificationReport::classes(){
    return _classes;
}

const std::chrono::high_resolution_clock::duration& ClassificationReport::time(){
    return _time;
}

size_t ClassificationReport::iterations(){
    return _iterations;
}

const vector<View*>& ClassificationReport::views(){
    return _views;
}

void ClassificationReport::recalc(){
    do{
        _classes = classify_with_views(G, _views);
        _iterations++;
    }
    while(recolor(_classes, G));
}

Isomorph::Isomorph(Graph& g, Graph& h, AfterStable mode,
                   std::function<void(bool)> callback_G,
                   std::function<void(bool)> callback_H) : _g(g, callback_G), _h(h, callback_H) {
    permutation s_g, s_h;
    for(auto& t : _g.classes()){
        if (!_h.classes().count(t.first)){
            return;
        } else {
            if(_h.classes().at(t.first).size() != t.second.size()){
                return;
            }
        }
    }
    switch (mode) {
    case AfterStable::None:
        break;
    case AfterStable::Bruteforce:
        _perm = brute_force_classified(_g.classes(), _h.classes(), g, h);
        return;
        break;
    case AfterStable::Destabilize:
        while(destabilize(_g.classes(), g) && destabilize(_h.classes(), h)){
            _g.recalc();
            _h.recalc();
        }
        for(auto& t : _g.classes()){
            if (!_h.classes().count(t.first)){
                return;
            } else {
                if(_h.classes().at(t.first).size() != t.second.size()){
                    return;
                }
            }
        }
        break;
    default:
        break;
    }
    s_g = concat(make_vector(_g.classes()));
    s_h = concat(make_vector(_h.classes()));
    _perm = get_permutation(s_g, s_h);
}

vector<vector<edge>> deconstruct(Graph& G)
{
    vector<vector<edge>> res;
    for(vertex i = 0; i < G.V().size(); i++){
        res.push_back(View(G, i).lexmst_edges());
    }
    return res;
}

permutation& Isomorph::perm()
{
    return _perm;
}

ClassificationReport& Isomorph::g()
{
    return _g;
}

ClassificationReport& Isomorph::h()
{
    return _h;
}

ClassificationReport& Isomorph::operator[](size_t t)
{
    if (t == 0){
        return _g;
    } else {
        return _h;
    }
}
