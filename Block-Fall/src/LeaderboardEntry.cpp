#include "LeaderboardEntry.h"

#include <utility>

LeaderboardEntry::LeaderboardEntry(unsigned long score, time_t lastPlayed, string playerName)
        : score(score), last_played(lastPlayed), player_name(std::move(playerName)) {}
