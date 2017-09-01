#pragma once

#include <array>
#include <vector>

struct Tetronimo {
    using DataType = std::array< std::array<bool,4> , 4>;
    
    DataType _data;

    Tetronimo();
    //Tetronimo( DataType data);
    Tetronimo( const char* data);
    static std::vector<Tetronimo> tetronimos;
    Tetronimo rotate();
};
