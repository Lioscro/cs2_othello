#include <iostream>
#include <cstdlib>
#include <cstring>
#include "player.hpp"
using namespace std;

int main(int argc, char *argv[]) {
    // Read in side the player is on.
    if (argc != 2)  {
        cerr << "usage: " << argv[0] << " side" << endl;
        exit(-1);
    }
    Side side = (!strcmp(argv[1], "Black")) ? BLACK : WHITE;

    // Initialize player.
    Player *player = new Player(side);

    // Tell java wrapper that we are done initializing.
    cout << "Init done" << endl;
    cout.flush();

    // These were used for testing
    /*
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;
    player->board->doMove(new Move(0, 0), side);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;
    player->board->doMove(new Move(3, 2), side);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;

    player->board->doMove(new Move(4, 2), side);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;

    player->board->doMove(new Move(2, 2), WHITE);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;

    player->board->doMove(new Move(2, 3), side);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;

    player->board->doMove(new Move(2, 4), WHITE);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;

    vector<Move*> available = player->board->getMoves(side);
    for (unsigned int i = 0; i < available.size(); i++)
    {
        cerr << available[i]->getX() << " " << available[i]->getY() << endl;
    }


    player->board->doMove(new Move(4, 2), WHITE);
    player->board->printBoard();
    cerr << player->board->countWhite() << " " << player->board->countBlack() << " " << player->board->countEmpty() << endl;
    */
    /*
    char boardData[8][8] = {
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', 'b', ' ', ' ', ' ', ' ', ' ', ' '},
        {'b', 'w', 'b', 'b', 'b', 'b', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}
    };
    Board *board = new Board();
    board->setBoard(boardData);
    board->printBoard();
    cerr << board->count(BLACK) << " " << board->count(WHITE) << endl;
    cerr << player->calcScore(board) << endl;


    vector<Move*> available = board->getMoves(WHITE);
    cerr << available.size() << endl;

    cerr << player->calcMinScore(board->copy(), available[0], WHITE, 1) << endl;
    cerr << player->calcMinScore(board->copy(), available[1], WHITE, 1) << endl;
    */
    
    int moveX, moveY, msLeft;

    // Get opponent's move and time left for player each turn.
    while (cin >> moveX >> moveY >> msLeft) {
        Move *opponentsMove = nullptr;
        if (moveX >= 0 && moveY >= 0) {
            opponentsMove = new Move(moveX, moveY);
        }

        // Get player's move and output to java wrapper.
        Move *playersMove = player->doMove(opponentsMove, msLeft);
        if (playersMove != nullptr) {
            cout << playersMove->x << " " << playersMove->y << endl;
        } else {
            cout << "-1 -1" << endl;
        }
        cout.flush();
        cerr.flush();

        // Delete move objects.
        if (opponentsMove != nullptr) delete opponentsMove;
        if (playersMove != nullptr) delete playersMove;
    }

    return 0;
}
