#include "Chess.h"
#include "Board/Board.h"
#include "MoveRecommender/MoveRecommender.h"

int main()
{
    string board = "RNBQKBNRPPPPPPPP################################pppppppprnbqkbnr";
    Chess a(board);
    Board chessBoard(board);
    MoveRecommender recommender(chessBoard, 2);
    // Get and print the top 3 recommended moves before each turn
    recommender.recommendMoves();
    int codeResponse = 0;
    auto printFunc = [&recommender]() { recommender.printRecommendations(); };
    string res = a.getInput(printFunc);
    while (res != "exit")
    {
        

        /*
        codeResponse value :
        Illegal movements :
        11 - there is not piece at the source
        12 - the piece in the source is piece of your opponent
        13 - there one of your pieces at the destination
        21 - illegal movement of that piece
        31 - this movement will cause you checkmate
        legal movements :
        41 - the last movement was legal and cause check
        42 - the last movement was legal, next turn
        */

        std::string source = res.substr(0, 2);
        std::string dest = res.substr(2, 2);
        codeResponse = chessBoard.validateMove(source, dest);

        if (codeResponse == 41 || codeResponse == 42) {
            chessBoard.makeMove(source, dest);
            recommender.recommendMoves();

        }

        a.setCodeResponse(codeResponse);
        res = a.getInput(printFunc);
    }

    cout << endl << "Exiting " << endl;
    return 0;
}