#include "automata.h"
#include "types.h"
#include <iostream>

Automata::Automata()
    : size_x { 80 }, size_y { 80 },
      rule{Constants::DEFAULT_RULE} {
          std::vector<uint8_t> row(size_x);
          for(int i = 0;i<size_y;++i) {
              cells.push_back(row);
          };
          cells[0][size_x / 2] = 1;
          for( auto x = 0;x<cells.size()-1;++x) {
              auto& pr = cells[x];
              auto& row = cells[x+1];
              for (int j = 0;j<size_x;++j) {
                  auto bitpos = pr[(j + 1) % size_x] + pr[j] * 2 + pr[(j-1) % size_x ] * 4;
                  //std::cout  << "Bit pos is " << bitpos << std::endl;
                  row[j] = (rule >> bitpos) & 1;
                  //std::cout << "Row val is " << std::to_string(row[j]) << std::endl;
              };
          };
      };
