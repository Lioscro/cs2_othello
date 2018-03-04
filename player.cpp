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
     side = side;
     opponentsSide = (side == BLACK) ? WHITE : BLACK;
     
     // initialize board
     board = new Board();
     
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
	
	// set depth to 2 if minimax, otherwise set depth to...? used 4 here
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
	
	// otherwise, calculate the score of each valid move and choose the
	// move with the minimum score
	else {
		Move* bestMove = available[0];
		// NOTE: setting use_heuristic to true, so heuristic function
		// is being used here to calculate min scores
		int min = calcMinScore(board->copy(), available[0], side, depth, true);
		
		for (int i = 1; i < available.size(); i++) {
			// NOTE: setting use_heuristic to true, so heuristic function
			// is being used here to calculate min scores
			int score = calcMinScore(board->copy(); available[i], side, depth, true);
			
			if (score < min) {
				min = score
				bestMove = available[i];
			}
		}
		
		// delete all moves in possibleMoves except for the best one
		for (int i = 0; i < available.size(); i++) {
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
 */
Move *Player::doMoveMinimax(vector<Move*> moves, int depth, int msLeft)
{
    
}

/**
 * Calculates the minimum score if the given move is performed
 * on the given board by the given side.
 */
// NOTE: changed arguments to also include boolean use_heuristic (set to
// true if using heuristc function
int Player::calcMinScore(Board *copy, Move *move, Side side, int depth, bool use_heuristic)
{
    // Do the move.
    copy->doMove(move, side);
    // copy->printBoard();

    // Base case.
    if (depth <= 0)
    {
        int score;
        
        // if using heuristic function, use heuristic_calcScore
        // otherwise, just do normal calcScore
        if (use_heuristic) {
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

    int minscore = calcMinScore(copy->copy(), available[0], other, depth-1, use_heuristic);
    for (unsigned int i = 1; i < available.size(); i++)
    {
        int score = calcMinScore(copy->copy(), available[i], other, depth-1, use_heuristic);
        // cerr << score << endl;
        if (score < minscore)
        {
            minscore = score;
        }
    }

    // Free memory.
    delete copy;
    for (int i = 0; i < available.size(); i++) {
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
    Side other = (side == BLACK) ? WHITE : BLACK;
    return board->count(side) - board->count(other);
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
			
			// first check if spot is occupied and if so, which side
			// is occupying it
			if (board->occupied(i, j)) {
				
				// if your own side, then you gain the values outlined
				// in the heuristic_values array
				if (board->get(i, j) == side) {
					sum += heuristic_values[i][j];
				}
				
				// otherwise, you lose the values outlined in the
				// heuristic values array
				else {
					sum -= heuristic_values[i][j]
				}
			}
		}
	}
	
	return sum;
}


