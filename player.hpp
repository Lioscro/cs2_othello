#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

private:
    Side side;
    Side opponentsSide;

    // initialize array containing heuristic scores for each spot on board
    // NOTE: used random values here, should figure out optimal ones
    int heuristic_values[8][8] = {
    {100, -50, 25, 25, 25, 25, -50, 100},
    {-50, -75,  0,  0,  0,  0, -75, -50},
    { 25,   0,  0,  0,  0,  0,   0,  25},
    { 15,   0,  0,  0,  0,  0,   0,  15},
    { 15,   0,  0,  0,  0,  0,   0,  15},
    { 25,   0,  0,  0,  0,  0,   0,  25},
    {-50, -75,  0,  0,  0,  0, -75, -50},
    {100, -50, 25, 25, 25, 25, -50, 100}};

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
