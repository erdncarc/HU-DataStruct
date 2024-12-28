#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>

using namespace std;

class Block {
public:
    vector<vector<bool>> shape;
    Block *right_rotation = nullptr;
    Block *left_rotation = nullptr;
    Block *next_block = nullptr;
    int row{};
    int col{};
};

#endif
