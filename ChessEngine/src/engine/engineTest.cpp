#include "engineTest.hpp"


/*
runs a given number of games between two engines, switching the colors between each one and records the result of every game

*/
	void EngineTest::runGames(Engine& engine1, Engine& engine2, int numGames) {
        int numGamesCopy = numGames;
        int turn = 1;
        while (numGamesCopy > 0) {
            runOneGame(engine1, engine2, turn);
            turn ^= 1;
            numGamesCopy--;
        }
        cout << "engine1 score:" << engine1Score << "/" << numGames << "  percent:" << (engine1Score*100) / numGames << "%" << endl;
        cout << "engine2 score:" << engine2Score << "/" << numGames << "  percent:" << (engine2Score*100) / numGames << "%" << endl;
        cout << "draw percent:" << (numGames - (engine1Score + engine2Score)) * 100 / numGames << "%" << endl;
        cout << "total moves played:" << totalMovesOverall << endl;

	}

//runs one game between two engines and returns its result
	void EngineTest::runOneGame(Engine& engine1, Engine& engine2, int turn) {
        int status = -1;
        int move50Count = 0;
        int lastNumPiece = 0;
        int totalMoves = 0;
        int initTurn = turn;
        string currPosition = startString;
        while (status == -1) {
            totalMoves++;
            totalMovesOverall++;
            if (turn) {
                
                engine1.executeCmd("position" + currPosition);
                engine1.executeCmd("go");
                currPosition = engine1.getPos();
                status = engine1.getStatus();
                if (engine1.getPieceMove() != 2 && lastNumPiece == engine1.getNumPieces()) {
                    move50Count++;
                }
                else {
                    move50Count = 0;
                }
                lastNumPiece = engine1.getNumPieces();
            }
            else {
                engine2.executeCmd("position" + currPosition);
                engine2.executeCmd("go");
                currPosition = engine2.getPos();
                status = engine2.getStatus();
                if (engine2.getPieceMove() != 2 && lastNumPiece == engine2.getNumPieces()) {
                    move50Count++;
                }
                else {
                    move50Count = 0;
                }
                lastNumPiece = engine2.getNumPieces();
            }

            turn ^= 1;
            if (move50Count == 100) {
                status = 0;
            }
        }

        if (status && (turn)) {
            engine1Score++;
        }
        else if (status && !(turn)) {
            engine2Score++;
        }

	}

/*
meant to be used to test with an outside engine using the terminal, but currently unused and untested completely
*/
    int EngineTest::processInfoScore(const string& engineOutput) {
        istringstream iss(engineOutput);
        string token;
        while (iss >> token) {
            if (token == "score") {
                iss >> token;
                if (token == "mate") {
                    int mateIn;
                    iss >> mateIn;
                    // Checkmate is found if mateIn is not zero
                    return mateIn;
                }
            }
        }
        return -1;
    }



// constructor that starts the engine test
	EngineTest::EngineTest(Engine& engine1, Engine& engine2, int numGames, string startPosition) {
		engine1Score = 0;
		engine2Score = 0;
        totalMovesOverall = 0;
		startString = startPosition;
		runGames(engine1, engine2, numGames);		
	}
