#include <fstream>
#include <utility>
#include <sstream>
#include "BlockFall.h"

BlockFall::BlockFall(const string &grid_file_name, const string &blocks_file_name, bool gravity_mode, const string &leaderboard_file_name, string player_name)
        : gravity_mode(gravity_mode), leaderboard_file_name(leaderboard_file_name), player_name(std::move(player_name)) {
    read_blocks(blocks_file_name);
    initialize_grid(grid_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    ifstream file(input_file);
    string line;
    vector<vector<vector<bool>>> vBlocks;
    vector<vector<bool>> vBlock;
    vector<pair<int, int>> row_col;
    int row{};

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty()) continue;

            bool isFinish = false;
            vector<bool> vBlockLine;
            string blockLine;
            row++;

            if (line[0] == '[' && line[line.size() - 1] == ']') {
                blockLine = line.substr(1, line.size() - 2);
                isFinish = true;
            } else if (line[0] == '[') {
                blockLine = line.substr(1);
            } else if (line[line.size() - 1] == ']') {
                blockLine = line.substr(0, line.size() - 1);
                isFinish = true;
            } else {
                blockLine = line;
            }

            for (char i: blockLine) {
                vBlockLine.push_back(i == '1');
            }
            vBlock.push_back(vBlockLine);

            if (isFinish) {
                vBlocks.push_back(vBlock);
                row_col.emplace_back(row, blockLine.size());
                vBlock.clear();
                row = 0;
            }
        }
        file.close();

        power_up = vBlocks[vBlocks.size() - 1];
        vBlocks.pop_back();
        row_col.pop_back();

        initial_block = new Block;
        Block *current = initial_block;
        while (!vBlocks.empty()) {
            current->shape = vBlocks[0];
            vBlocks.erase(vBlocks.begin());

            current->row = row_col[0].first;
            current->col = row_col[0].second;
            row_col.erase(row_col.begin());

            if (!vBlocks.empty()) {
                current->next_block = new Block;
                current = current->next_block;
            }
        }
        rotateBlock();
        active_rotation = initial_block;
    }
}

void BlockFall::initialize_grid(const string &input_file) {
    ifstream file(input_file);
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            vector<int> vGrid;
            int number;

            while (ss >> number) {
                vGrid.push_back(number);
            }
            grid.push_back(vGrid);
        }
        file.close();
    }
    rows = (int) grid.size();
    cols = (int) grid[0].size();
}

void BlockFall::rotateBlock() const {
    Block *block = initial_block;
    while (block != nullptr) {
        Block *current = block;
        do {
            vector<vector<bool>> vBlocks;
            for (int i = 0; i < current->col; ++i) {
                vector<bool> vBlock;
                vBlock.reserve(current->row);
                for (int j = 0; j < current->row; ++j) {
                    vBlock.push_back(current->shape[current->row - 1 - j][i]);
                }
                vBlocks.push_back(vBlock);
            }

            if (vBlocks == block->shape) {
                current->right_rotation = block;
                block->left_rotation = current;
            } else {
                current->right_rotation = new Block;
                current->right_rotation->shape = vBlocks;
                current->right_rotation->next_block = block->next_block;
                current->right_rotation->left_rotation = current;
                current->right_rotation->row = current->col;
                current->right_rotation->col = current->row;
            }
            current = current->right_rotation;
        } while (current != block);

        block = block->next_block;
    }
}

BlockFall::~BlockFall() {
    Block *current = initial_block;
    while (current != nullptr) {
        Block *temp1 = current->next_block;
        while (current->right_rotation != current) {
            Block *temp2 = current->right_rotation;
            temp2->right_rotation->left_rotation = current;
            current->right_rotation = temp2->right_rotation;
            delete temp2;
        }
        delete current;
        current = temp1;
    }
}
