
#include "tetronimo.h"
#include <iostream>

Tetronimo::Tetronimo() : _data{} {}

// Tetronimo::Tetronimo( DataType data) : _data(data) {
//     std::cout << "Bing" << std::endl;
// }

Tetronimo::Tetronimo( const char* data) : _data {}  {
    std::cout << "Ctor" << std::endl;
    for ( int i = 0;i<4;++i) {
        for(int j = 0;j<4; ++j) {
            if (data[i * 4 + j]!=' ') {
                std::cout << "Boom" << std::endl;
                _data[i][j] = true;
            }
        }
    }
}

Tetronimo Tetronimo::rotate() {
    Tetronimo ret;
    for (int i = 0;i<4;++i) {
        for (int j = 0;j<4;++j) {
            if (_data[i][j]) {
                ret._data[j][i] = true;
            };
        };
    };
    return ret;
    
};

//clang-format off
std::vector<Tetronimo> Tetronimo::tetronimos = {
    "    "
    "    "
    "****"
    "    ",
    
    "    "
    "*** "
    "  * "
    "    ",

    "    "
    " ** "
    " ** "
    "    ",
    
    "    "
    " ** "
    "**  "
    "    ",
    
    "    "
    "*** "
    " *  "
    "    ",
    
    "    "
    "**  "
    " ** "
    "    ",
};

//clang-format on
