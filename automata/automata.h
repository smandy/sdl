#pragma once
#include <cstdint>
#include <vector>

class Automata {
    uint32_t size_x;
    uint32_t size_y;
public:
    int32_t rule;
    std::vector< std::vector<uint8_t> > cells;
    
    Automata();
};
