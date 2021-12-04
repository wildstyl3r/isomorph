#ifndef LEXMSTNODE_HPP
#define LEXMSTNODE_HPP
#include "isomorph.hpp"
#include <string>
#include <algorithm>
#include <vector>

struct LexMSTNode 
{
    bool used;
    value inlet_degree;
    value outlet_degree;
    streeng_symbol color;
    LexMSTNode* parent;
    vector<LexMSTNode*> children;

    streeng get_str();
    
    LexMSTNode(value i_d, value o_d, value color) : used(false), inlet_degree(i_d),  outlet_degree(o_d), color(color), parent(nullptr) {}
};
#endif
