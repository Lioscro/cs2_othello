#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

private:
    Side side;
    Move *doMoveMinimax(vector<Move*> moves, int depth, int msLeft, bool heuristic);
    int calcHeuristicScore(Board *board);

public:
    Board *board;

    Player(Side side);
    ~Player();

    int calcScore(Board *board);
    int calcMinScore(Board *copy, Move *move, Side side, int depth, bool heuristic);
    Move *doMove(Move *opponentsMove, int msLeft, int nthreads);
    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
