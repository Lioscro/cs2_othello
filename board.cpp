#include "board.hpp"
#include <iostream>

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    // Occupy counts.
    // No pieces have been added yet, so there are 64 empty spaces.
    counts[EMPTY] = 64;
    counts[WHITE] = 0;
    counts[BLACK] = 0;

    // Set whites at (3,3) and (4,4)
    set(WHITE, 3, 3);
    set(WHITE, 4, 4);

    // Set blacks at (3,4) and (4,3)
    set(BLACK, 3, 4);
    set(BLACK, 4, 3);
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    std::copy(&board[0][0], &board[0][0] + 8 * 8, &newBoard->board[0][0]);
    std::copy(begin(counts), end(counts), begin(newBoard->counts));
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return board[y][x] != EMPTY;
}

int Board::get(int x, int y) {
    return board[y][x];
}

void Board::set(Side side, int x, int y) {
    Side other = (side == BLACK) ? WHITE : BLACK;

    // First, update the counts.
    counts[side]++;

    if (get(x, y) == other)
    {
        counts[other]--;
    }
    else
    {
        counts[EMPTY]--;
    }

    // Then place the piece.
    board[y][x] = side;

}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && (get(x, y) == other)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && (get(x, y) == other));

                if (onBoard(x, y) && (get(x, y) == side)) return true;
            }
        }
    }
    return false;
}

/**
 * Returns a Capture object specifying whether the move
 * is valid & (if valid) the moves that are made due to the capture.
 */
Capture Board::checkMoveCapture(Move *m, Side side)
{
    Capture result;

    // Passing is only legal if you have no moves.
    if (m == nullptr) {
        result.valid = !hasMoves(side);
        return result;
    }

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return result;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            vector<Move*> captures;
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && (get(x, y) == other)) {
                do {
                    // Make new move.
                    Move *newMove = new Move(x, y);
                    captures.push_back(newMove);
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && (get(x, y) == other));

                if (onBoard(x, y) && (get(x, y) == side)) {
                    // Add these moves to captures.
                    for (unsigned int i = 0; i < captures.size(); i++)
                    {
                        result.captures.push_back(captures[i]);
                    }
                    result.valid = true;
                }
            }
        }
    }
    return result;
}

/**
 * Returns all available moves for the specified side.
 */
vector<Move*> Board::getMoves(Side side)
{
    // The results.
    vector<Move*> results;

    // Loop through every position and call checkMove.
    for (int y = 0; y < 8; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            // Make the new move.
            Move *newMove = new Move(x, y);
            // cerr << "checking move: " << newMove->getX() << " " << newMove->getY() <<endl;

            // Check if the new move is valid for side.
            if (checkMove(newMove, side))
            {
                results.push_back(newMove);
            }
            else
            {
                delete newMove;
            }
        }
    }

    return results;
}


/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Check the move is valid AND get moves required.
    Capture check = checkMoveCapture(m, side);

    int X = m->getX();
    int Y = m->getY();

    if (check.valid)
    {
        // First, make moves in check.
        for (unsigned int i = 0; i < check.captures.size(); i++)
        {
            int x = check.captures[i]->getX();
            int y = check.captures[i]->getY();
            set(side, x, y);

            // Free memory.
            delete check.captures[i];
        }

        // Finally, make the move.
        set(side, X, Y);
    }
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return counts[BLACK];
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return counts[WHITE];
}

/*
 * Current count of emtpy spaces.
 */
int Board::countEmpty() {
    return counts[EMPTY];
}

/*
 * Prints current board to terminal.
 */
void Board::printBoard() {
    cerr << "-------------------" << endl;
    for (int y = 0; y < 8; y++) {
        cerr << "| ";
        for (int x = 0; x < 8; x++) {
            cerr << get(x, y) << ' ';
        }
        cerr << "|" << endl;
    }
    cerr << "-------------------" << endl;
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[8][8]) {

    // Clear counts.
    counts[EMPTY] = 64;
    counts[WHITE] = 0;
    counts[BLACK] = 0;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            board[y][x] = EMPTY;

            if (data[y][x] == 'b') {
                set(BLACK, x, y);
            }
            else if (data[y][x] == 'w') {
                set(WHITE, x, y);
            }
        }
    }
}
