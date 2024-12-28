#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Leaderboard.h"

void Leaderboard::insert_new_entry(LeaderboardEntry *new_entry) {
    if (head_leaderboard_entry == nullptr) head_leaderboard_entry = new_entry;

    LeaderboardEntry *entry = head_leaderboard_entry;
    LeaderboardEntry *preEntry = nullptr;
    int numE = 1;
    while (entry != nullptr) {
        if (new_entry->score > entry->score) {
            new_entry->next_leaderboard_entry = entry;
            if (preEntry == nullptr) {
                head_leaderboard_entry = new_entry;
                break;
            } else {
                preEntry->next_leaderboard_entry = new_entry;
                break;
            }
        }
        if (numE == numberEntry) {
            entry->next_leaderboard_entry = new_entry;
        }
        preEntry = entry;
        entry = entry->next_leaderboard_entry;
        numE++;
    }
    numberEntry++;

    entry = head_leaderboard_entry;
    if (numberEntry >= MAX_LEADERBOARD_SIZE) {
        for (int i = 1; i <= MAX_LEADERBOARD_SIZE; i++) {
            if (i == MAX_LEADERBOARD_SIZE) {
                delete entry->next_leaderboard_entry;
                entry->next_leaderboard_entry = nullptr;
            }
            entry = entry->next_leaderboard_entry;
        }
    }
}

void Leaderboard::write_to_file(const string &filename) const {
    ofstream file(filename);

    if (file.is_open() && head_leaderboard_entry != nullptr) {
        LeaderboardEntry *leaderboardEntry = head_leaderboard_entry;

        while (leaderboardEntry != nullptr) {
            file << leaderboardEntry->score << " " << leaderboardEntry->last_played << " " << leaderboardEntry->player_name << "\n";
            leaderboardEntry = leaderboardEntry->next_leaderboard_entry;
        }
        file.close();
    }
}

void Leaderboard::read_from_file(const string &filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    LeaderboardEntry *leaderboardEntry = head_leaderboard_entry;
    int numE{};
    while (getline(file, line) && numE < MAX_LEADERBOARD_SIZE) {
        if (line.empty()) continue;

        istringstream ss(line);
        vector<string> words;
        string word;
        while (ss >> word) {
            words.push_back(word);
        }

        auto *entry = new LeaderboardEntry(stoul(words[0]), stol(words[1]), words[2]);

        if (head_leaderboard_entry == nullptr) {
            head_leaderboard_entry = entry;
            leaderboardEntry = entry;
        } else {
            leaderboardEntry->next_leaderboard_entry = entry;
            leaderboardEntry = entry;
        }
        numE++;
        numberEntry++;
    }
    file.close();
}

void Leaderboard::print_leaderboard() const {
    cout << "Leaderboard\n-----------\n";

    int order = 1;
    LeaderboardEntry *leaderboardEntry = head_leaderboard_entry;
    while (leaderboardEntry != nullptr) {
        cout << order << ". " << leaderboardEntry->player_name << " " << leaderboardEntry->score << " " <<
             put_time(localtime(&leaderboardEntry->last_played), "%H:%M:%S/%d.%m.%Y") << "\n";

        leaderboardEntry = leaderboardEntry->next_leaderboard_entry;
        order++;
    }
}

Leaderboard::~Leaderboard() {
    LeaderboardEntry *current = head_leaderboard_entry;
    LeaderboardEntry *temp;
    while (current != nullptr) {
        temp = current->next_leaderboard_entry;
        delete current;
        current = temp;
    }
}
