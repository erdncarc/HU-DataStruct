#include <fstream>
#include <iostream>
#include "GameController.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

void GameController::printGrid(const vector<vector<int>> &grid) {
    for (const auto &i: grid) {
        for (int j: i) {
            cout << (j ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << "\n";
    }
    cout << "\n";
}

void GameController::cellFill(BlockFall &game, int fill, int numR, int numC) const {
    for (int i = 0; i < game.active_rotation->row; i++) {
        for (int j = 0; j < game.active_rotation->col; j++) {
            game.grid[i + numR][j + start + numC] = game.active_rotation->shape[i][j] ? fill : game.grid[i + numR][j + start + numC];
        }
    }
}

bool GameController::cellControl(BlockFall &game, int numR, int numC) const {
    for (int i = 0; i < game.active_rotation->row; i++) {
        for (int j = 0; j < game.active_rotation->col; j++) {
            if (game.grid[i + numR][j + start + numC] == 1 && game.active_rotation->shape[i][j]) return true;
        }
    }
    return false;
}

void GameController::rotate(BlockFall &game, bool clockwise) const {
    vector<vector<int>> firstGrid = game.grid;

    Block *rotation = clockwise ? game.active_rotation->right_rotation : game.active_rotation->left_rotation;
    cellFill(game, 0);

    if (start + rotation->col > game.cols) {
        game.grid = firstGrid;
        return;
    }

    for (int i = 0; i < rotation->row; i++) {
        for (int j = 0; j < rotation->col; j++) {
            if (game.grid[i][j + start] == 1 && rotation->shape[i][j]) {
                game.grid = firstGrid;
                return;
            }
        }
    }

    game.active_rotation = rotation;
    cellFill(game, 1);
}

void GameController::move(BlockFall &game, const int move) {
    vector<vector<int>> firstGrid = game.grid;

    if (start + move < 0 || start + move + game.active_rotation->col > game.cols) {
        return;
    }

    cellFill(game, 0);

    if (cellControl(game, 0, move)) {
        game.grid = firstGrid;
        return;
    }

    cellFill(game, 1, 0, move);

    start += move;
}

void GameController::drop(BlockFall &game) const {
    vector<vector<int>> firstGrid = game.grid;

    int numOne = 0;
    for (int i = 0; i < game.active_rotation->row; i++) {
        for (int j = 0; j < game.active_rotation->col; j++) {
            if (game.active_rotation->shape[i][j]) {
                numOne++;
            }
        }
    }

    int numFall = 1;
    while (numFall <= game.rows - game.active_rotation->row) {
        cellFill(game, 0, numFall - 1, 0);

        if (cellControl(game, numFall)) {
            game.grid = firstGrid;
            game.current_score += (numFall - 1) * numOne;
            game.high_score = game.current_score > game.high_score ? game.current_score : game.high_score;
            return;
        }

        cellFill(game, 1, numFall);

        firstGrid = game.grid;
        numFall++;
    }
    game.current_score += (numFall - 1) * numOne;
    game.high_score = game.current_score > game.high_score ? game.current_score : game.high_score;
}

void GameController::gravity(BlockFall &game) {
    if (game.gravity_mode) {
        for (int i = 0; i < game.cols; i++) {
            int numOne{};
            for (int j = 0; j < game.rows; j++) {
                if (game.grid[j][i] == 1) {
                    game.grid[j][i] = 0;
                    numOne++;
                }
            }
            int rows = game.rows - 1;
            while (numOne != 0) {
                game.grid[rows][i] = 1;
                numOne--;
                rows--;
            }
        }
    }
}

void GameController::clearRow(BlockFall &game, bool command) {
    vector<vector<int>> firstGrid = game.grid;

    bool clearing = false;
    for (int i = 0; i < game.rows; i++) {
        bool isExist = true;
        for (int j = 0; j < game.cols; j++) {
            if (game.grid[i][j] == 0) {
                isExist = false;
            }
        }
        if (isExist) {
            clearing = true;
            game.current_score += game.cols;
            game.high_score = game.current_score > game.high_score ? game.current_score : game.high_score;
            for (int j = 0; j < i; j++) {
                for (int k = 0; k < game.cols; k++) {
                    game.grid[i - j][k] = game.grid[i - j - 1][k];
                    game.grid[i - j - 1][k] = 0;
                }
            }
        }
    }

    if (clearing && command) {
        cout << "\nBefore clearing:\n";
        printGrid(firstGrid);
    }
}

void GameController::powerUp(BlockFall &game) {
    int rows = (int) game.power_up.size();
    int cols = (int) game.power_up[0].size();
    for (int i = 0; i <= game.rows - rows; i++) {
        for (int j = 0; j <= game.cols - cols; j++) {
            int matchingCells{};
            for (int k = 0; k < rows; k++) {
                for (int l = 0; l < cols; l++) {
                    if (game.grid[i + k][j + l] == game.power_up[k][l])
                        matchingCells++;
                }
            }

            if (matchingCells == rows * cols) {
                int numOne = 0;
                for (int m = 0; m < game.rows; m++) {
                    for (int n = 0; n < game.cols; n++) {
                        if (game.grid[m][n]) {
                            numOne++;
                        }
                    }
                }

                game.current_score += 1000 + numOne;
                game.high_score = game.current_score > game.high_score ? game.current_score : game.high_score;
                cout << "\nBefore clearing:\n";
                printGrid(game.grid);
                game.grid = vector<vector<int>>(game.rows, vector<int>(game.cols, 0));
                return;
            }
        }
    }
}

void GameController::finish(BlockFall &game) {
    auto *entry = new LeaderboardEntry(game.current_score, time(nullptr), game.player_name);
    game.leaderboard.insert_new_entry(entry);
    game.leaderboard.write_to_file(game.leaderboard_file_name);
    game.leaderboard.print_leaderboard();
}

void GameController::gameOver(BlockFall &game) {
    cout << "\nGAME OVER!\nNext block that couldn't fit:\n";
    for (const auto &i: game.active_rotation->shape) {
        for (int j: i) {
            cout << (j ? occupiedCellChar : unoccupiedCellChar);
        }
        cout << "\n";
    }
    cout << "\nFinal grid and score:\n\nScore: " << game.current_score << "\nHigh Score: " << game.high_score << "\n";
    printGrid(game.grid);

    finish(game);
    start = 0;
}

bool GameController::play(BlockFall &game, const string &commands_file) {
    ifstream file(commands_file);
    string command;
    if (!file.is_open()) return true;

    game.high_score = game.leaderboard.head_leaderboard_entry->score;
    bool isExist = false;
    while (getline(file, command)) {
        if (command.empty()) continue;

        if (!isExist && command != "GRAVITY_SWITCH") {
            if (cellControl(game)) {
                gameOver(game);
                return false;
            }
            cellFill(game, 1);
            isExist = true;
        }

        if (command == "PRINT_GRID") {
            cout << "\nScore: " << game.current_score << "\nHigh Score: " << game.high_score << "\n";
            printGrid(game.grid);

        } else if (command == "ROTATE_RIGHT") {
            rotate(game, true);

        } else if (command == "ROTATE_LEFT") {
            rotate(game, false);

        } else if (command == "MOVE_RIGHT") {
            move(game, 1);

        } else if (command == "MOVE_LEFT") {
            move(game, -1);

        } else if (command == "DROP") {
            drop(game);
            start = 0;
            isExist = false;

            gravity(game);
            powerUp(game);
            clearRow(game, true);

            if (game.active_rotation->next_block != nullptr) {
                game.active_rotation = game.active_rotation->next_block;
            } else {
                cout << "\nYOU WIN!\nNo more blocks.\nFinal grid and score:\n\nScore: " << game.current_score << "\nHigh Score: " << game.high_score << "\n";
                printGrid(game.grid);
                finish(game);
                start = 0;
                return true;
            }

        } else if (command == "GRAVITY_SWITCH") {
            game.gravity_mode = !game.gravity_mode;
            cellFill(game, 0);
            gravity(game);
            clearRow(game, false);
            if (isExist) {
                cellFill(game, 1);
            }

        } else {
            cout << "\nUnknown command: " << command << "\n\n\n";
        }
    }
    file.close();

    cellFill(game, 0);
    cout << "\nGAME FINISHED!\nNo more commands.\nFinal grid and score:\n\nScore: " << game.current_score << "\nHigh Score: " << game.high_score << "\n";
    printGrid(game.grid);

    finish(game);
    start = 0;
    return true;
}
