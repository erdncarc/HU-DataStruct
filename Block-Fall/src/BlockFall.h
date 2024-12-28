#ifndef BLOCKFALL_H
#define BLOCKFALL_H

#include <vector>
#include <string>

#include "Block.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"

using namespace std;

class BlockFall {
public:
    BlockFall(const string &grid_file_name, const string &blocks_file_name, bool gravity_mode, const string &leaderboard_file_name, string player_name);

    void read_blocks(const string &input_file);

    void initialize_grid(const string &input_file);

    void rotateBlock() const;

    virtual ~BlockFall();

    int rows{};
    int cols{};
    vector<vector<int>> grid;
    vector<vector<bool>> power_up;
    Block *initial_block = nullptr;
    Block *active_rotation = nullptr;
    bool gravity_mode = false;
    unsigned long current_score{};
    unsigned long high_score{};
    string leaderboard_file_name;
    string player_name;
    Leaderboard leaderboard;
};

#endif
