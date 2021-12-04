#include "lex_mst_node.hpp"

streeng LexMSTNode::get_str() 
{
    streeng res = { -1, }; // (

    vector<streeng> children_strs(children.size());
    
    for (size_t i = 0; i < children.size(); ++i){
	children_strs[i] = children[i]->get_str();
    }
	
    std::sort(children_strs.begin(), children_strs.end());
	
    for (streeng s:children_strs){
	res += s;
    }
    res += color; // )
    
    return res;
}
