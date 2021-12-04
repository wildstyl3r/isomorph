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

    streeng getStr();
    
    LexMSTNode(value i_d, value o_d, value color) : used(false), inlet_degree(i_d),  outlet_degree(o_d), color(color), parent(nullptr) {}
};

streeng LexMSTNode::getStr() 
{
    streeng res = { -1, }; // (

    vector<streeng> children_strs(children.size());
    
    for (size_t i = 0; i < children.size(); ++i){
	children_strs[i] = children[i]->getStr();
    }
	
    std::sort(children_strs.begin(), children_strs.end());
	
    for (streeng s:children_strs){
	res += s;
    }
    res += color; // )
    
    return res;
}
#endif
