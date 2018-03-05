#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // define the color of your side and opponent's side
    this->side = side;
    opponentsSide = (side == BLACK) ? WHITE : BLACK;

    // initialize board
    board = new Board();

}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
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
Move *Player::doMove(Move *opponentsMove, int msLeft, int nthreads) {
    // modify the board so it includes opponent's move
  	board->doMove(opponentsMove, opponentsSide);

  	// get all valid moves - if there are none, just return nullptr
  	vector<Move*> available = board->getMoves(side);
    int nmoves = available.size();
    if (nmoves == 0) {
  		  return nullptr;
  	}

    // whether or not we are using minimax
    int depth = testingMinimax ? 2 : 6;
    bool heuristic = true;

    // Multithreading.
    if (nthreads <= 1)
    {
        Move *nextMove = doMoveMinimax(available, depth, msLeft, heuristic);
    }
    else
    {
        vector<Move*> availables[nthreads];
        for (unsigned int i = 0; i < nmoves; i++)
        {
            
        }
    }


    // Before returning, perform the move.
    board->doMove(nextMove, side);
    return nextMove;
}

/**
 * Computes next move using Minimax algorithm.
 * Assumes the vector moves has > 0 size.
 */
Move *Player::doMoveMinimax(vector<Move*> available, int depth, int msLeft, bool heuristic)
{
    Move* bestMove = available[0];
    // NOTE: setting use_heuristic to true, so heuristic function
    // is being used here to calculate min scores
    int minimax = calcMinScore(board->copy(), available[0], side, depth-1, heuristic);

    for (unsigned int i = 1; i < available.size(); i++) {
        // NOTE: setting use_heuristic to true, so heuristic function
        // is being used here to calculate min scores
        int score = calcMinScore(board->copy(), available[i], side, depth-1, heuristic);

        if (score > minimax) {
            minimax = score;
            bestMove = available[i];
        }
    }

    // Once we've found the best move, delete all the other ones.
    // delete all moves in possible Moves except for the best one
    for (unsigned int i = 0; i < available.size(); i++) {
        if (available[i] != bestMove) {
            delete available[i];
        }
    }
    return bestMove;
}

/**
 * Calculates the minimum score if the given move is performed
 * on the given board by the given side.
 */
// NOTE: changed arguments to also include boolean use_heuristic (set to
// true if using heuristc function
int Player::calcMinScore(Board *copy, Move *move, Side side, int depth, bool heuristic)
{
    // Do the move.
    copy->doMove(move, side);

    Side other = (side == BLACK) ? WHITE : BLACK;
    vector<Move*> available = copy->getMoves(other);

    // Base case.
    if (depth <= 0 || available.size() == 0) {
        int score;

        // if using heuristic function, use heuristic_calcScore
        // otherwise, just do normal calcScore
        if (heuristic) {
			      score = calcHeuristicScore(copy);
		    }
        else {
            score = calcScore(copy);
        }

        // free memory and return score
        delete copy;
        for (unsigned int i = 0; i < available.size(); i++) {
    		    delete available[i];
    	  }
        return score;
    }

    // Otherwise, recursively call this function for every available move.
    // First, get all vailable moves.
    int minscore = calcMinScore(copy->copy(), available[0], other, depth-1, heuristic);
    for (unsigned int i = 1; i < available.size(); i++) {
        int score = calcMinScore(copy->copy(), available[i], other, depth-1, heuristic);
        if (score < minscore) {
            minscore = score;
        }
    }

    // Free memory.
    delete copy;
    for (unsigned int i = 0; i < available.size(); i++) {
		    delete available[i];
	  }

    return minscore;
}

/**
 * Calculates the player's score on the given board by calculating
 * (# stones the player has) - (# stones the opponent has)
 */
int Player::calcScore(Board *board)
{
    return board->count(side) - board->count(opponentsSide);
}

/**
 * Calculates the player's score on the given board by using the
 * heuristic function, with different values assigned to different
 * spaces on the board
 */
int Player::calcHeuristicScore(Board *board)
{
  	int sum = 0;

  	// go through each spot on the board and calculate its values
  	// based on the heuristic values assigned in the Player constructor
  	for (int i = 0; i < 8; i++) {
    		for (int j = 0; j < 8; j++) {
    				// if your own side, then you gain the values outlined
    				// in the heuristic_values array
    				if (board->get(i, j) == side) {
      					sum += heuristic_values[i][j];
    				}

    				// otherwise, you lose the values outlined in the
    				// heuristic values array
    				else if (board->get(i, j) == opponentsSide){
      					sum -= heuristic_values[i][j];
    				}
    		}
  	}

  	return sum;
}


