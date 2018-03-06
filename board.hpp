#ifndef __BOARD_H__
#define __BOARD_H__

#include "common.hpp"
#include <vector>
using namespace std;

struct Capture {
    bool valid = false;
    vector<Move*> captures;
};

class Board {

private:
    // The first index is the row from the top (y).
    // The second index is the column from the left (x).
    int board[8][8] = { {EMPTY} };

    // The number of spaces on the board corresponding
    // to each Side value.
    // # of empty spaces = counts[EMPTY]
    // # of white spaces = counts[WHITE]
    // # of black spaces = counts[BLACK]
    int counts[3];

<<<<<<< HEAD
=======
    bool occupied(int x, int y);
>>>>>>> f2c0ca8ce1625f2da7a26c9fb28b8bcbb5c04ec7
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();
    int get(int x, int y);

	bool occupied(int x, int y);
    int get(int x, int y);
    
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    Capture checkMoveCapture(Move *m, Side side);
    void doMove(Move *m, Side side);
    vector<Move*> getMoves(Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    int countEmpty();

    void setBoard(char data[8][8]);
    void printBoard();
};

#endif
