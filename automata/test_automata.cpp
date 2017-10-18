#include "automata.h"
#include <iostream>

int main(int argc, char *argv[]) {
    Automata a;

    for(auto x : a.cells) {
        for(auto y : x) {
            std::cout << ( y ? "*" : " ");
        };
        std::cout << std::endl;
    };
}
