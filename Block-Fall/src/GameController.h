#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file);

    static void printGrid(const vector<vector<int>> &grid);

    void cellFill(BlockFall &game, int fill, int numR = 0, int numC = 0) const;

    bool cellControl(BlockFall &game, int num1 = 0, int num2 = 0) const;

    void rotate(BlockFall &game, bool clockwise) const;

    void move(BlockFall &game, int move);

    void drop(BlockFall &game) const;

    static void gravity(BlockFall &game);

    static void clearRow(BlockFall &game, bool command);

    static void powerUp(BlockFall &game);

    static void finish(BlockFall &game);

    void gameOver(BlockFall &game);

    int start{};
};

#endif
