#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // define the color of your side and opponent's side
    side = s;
    opponentsSide = (side == BLACK) ? WHITE : BLACK;

    // initialize board
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
	// modify the board so it includes opponent's move
	board->doMove(opponentsMove, opponentsSide);
	
	// set depth to 2 if minimax, otherwise set depth to 4 here
	int depth;
	if (testingMinimax) {
		depth = 2;
	}
	else {
		depth = 4;
	}
	
	// get all valid moves - if there are none, just return nullptr
	vector<Move*> available = board->getMoves(side);
	if (available.size() == 0) {
		return nullptr;
	}
	
	// otherwise, calculate the score of each valid move and choose the opponent
	// move with the minimum score
	else {
		Move* bestMove = available[0];
		// NOTE: setting use_heuristic to true, so heuristic function
		// is being used here to calculate min scores
		int minmax = calcMinScore(board->copy(), available[0], side, depth, true, true );
		for (unsigned int i = 1; i < available.size(); i++) {
			// NOTE: setting use_heuristic to true, so heuristic function
			// is being used here to calculate min scores
			int score = calcMinScore(board->copy(), available[i], side, depth, true, true);
			
			if (score > minmax) { // calculate max score for the current player
				minmax = score;
				bestMove = available[i];
			}
		}
		
		// delete all moves in possibleMoves except for the best one
		for (unsigned int i = 0; i < available.size(); i++) {
			if (available[i] != bestMove) {
				delete available[i];
			}
		}
		
		// return move with minimum score
		return bestMove;
	}
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
    int minimax = calcMinScore(board->copy(), available[0], side, depth-1, heuristic, true);

    for (unsigned int i = 1; i < available.size(); i++) {
        // NOTE: setting use_heuristic to true, so heuristic function
        // is being used here to calculate min scores
        int score = calcMinScore(board->copy(), available[i], side, depth-1, heuristic, true);

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
int Player::calcMinScore(Board *copy, Move *move, Side side, int depth, bool heuristic, bool getMin)
{
    // Do the move.
    copy->doMove(move, side);

    // Base case.
    if (depth <= 0) {
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
        return score;
    }

    // Otherwise, recursively call this function for every available move.
    // First, get all vailable moves.
    Side other = (side == BLACK) ? WHITE : BLACK;
    vector<Move*> available = copy->getMoves(other);
    if (available.size() == 0) {
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
        return score;
    }
    else {
		int minmaxscore = calcMinScore(copy->copy(), available[0], other, depth-1, heuristic, !getMin);
		for (unsigned int i = 1; i < available.size(); i++) {
			int score = calcMinScore(copy->copy(), available[i], other, depth-1, heuristic, !getMin);
			if (getMin && (score < minmaxscore)) {
				minmaxscore = score;
			}
			else if (!getMin && (score > minmaxscore)){
				minmaxscore = score;
			}
		}

		// Free memory.
		delete copy;
		for (unsigned int i = 0; i < available.size(); i++) {
			delete available[i];
		}
	  
	  return minmaxscore;
  }
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


