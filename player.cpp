#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
     side = side;
     board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */
    return nullptr;
}

/**
 * Computes next move using Minimax algorithm.
 */
Move *Player::doMoveMinimax(vector<Move*> moves, int depth, int msLeft)
{
    
}

/**
 * Calculates the minimum score if the given move is performed
 * on the given board by the given side.
 */
int Player::calcMinScore(Board *copy, Move *move, Side side, int depth)
{
    // Do the move.
    copy->doMove(move, side);
    // copy->printBoard();

    // Base case.
    if (depth <= 0)
    {
        // Return the score.
        int score = calcScore(copy);
        delete copy;
        return score;
    }

    // Otherwise, recursively call this function for every available move.
    // First, get all vailable moves.
    Side other = (side == BLACK) ? WHITE : BLACK;
    vector<Move*> available = copy->getMoves(other);

    int minscore = calcMinScore(copy->copy(), available[0], other, depth-1);
    for (unsigned int i = 1; i < available.size(); i++)
    {
        int score = calcMinScore(copy->copy(), available[i], other, depth-1);
        // cerr << score << endl;
        if (score < minscore)
        {
            minscore = score;
        }
    }

    // Free memory.
    delete copy;

    return minscore;
}

/**
 * Calculates the player's score on the given board by calculating
 * (# stones the player has) - (# stones the opponent has)
 */
int Player::calcScore(Board *board)
{
    Side other = (side == BLACK) ? WHITE : BLACK;
    return board->count(side) - board->count(other);
}


