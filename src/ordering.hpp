#ifndef ORDERING_HPP
#define ORDERING_HPP
#include "../../grypho/include/grypho.hpp"
#include <map>
#include <vector>

class VxOrder
{
    std::map<edge, int>_ORDER;

public:
    void set(vertex lhs, vertex rhs, int ord)
	{
	    if (lhs == rhs) return;
	    if (lhs < rhs) {
		_ORDER[{lhs, rhs}] = ord;
	    } else {
		_ORDER[{rhs, lhs}] = -ord;
	    }
	}

    int get(vertex lhs, vertex rhs)
	{
	    if (lhs == rhs) return 0;
	    if (lhs < rhs) {
		return _ORDER[{lhs, rhs}];
	    } else {
		return -_ORDER[{rhs, lhs}];
	    }	    
	}

    int childset_compare(vector<vertex>& lhs, vector<vertex>& rhs)
	{
	    int sub = std::min(lhs.size(), rhs.size());
	    for(int i = 0; i < sub; i++){
		int comp = get(lhs[i], rhs[i]);
		if (comp == 0)
		    continue;
		else
		    return comp;
	    }
	    if (lhs.size() < rhs.size()) {
		return -1;
	    }
	    if (lhs.size() > rhs.size()) {
		return +1;
	    }
	    return 0;
	}
};
#endif
