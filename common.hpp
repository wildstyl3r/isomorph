#ifndef COMMON_HPP
#define COMMON_HPP

#include "grypho.hpp"
#include <functional>
#include <map>
#include <chrono>
#include <string>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <list>

using morph_iter_callback_t = std::function<void()>;
using streeng_symbol        = value;
using streeng               = std::basic_string<streeng_symbol>;
using classification        = std::map< streeng, vector<vertex> >;

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

enum class AfterStable {
    None,
    Bruteforce,
    Destabilize,
};

#endif
