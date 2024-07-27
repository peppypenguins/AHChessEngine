#include "engineRandom.hpp"


//simply finds a random legal move
	void EngineRandom::handleGo (const string& input){
			vector<string> foundMoves = findLegalMoves();

	if (legalMoves.size() > 0) {
		vector<string> allMoves(foundMoves.begin(), foundMoves.end());
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distr(0, allMoves.size() - 1);
		int randomIndex = distr(gen);
		bestMove = allMoves[randomIndex];
		updateBoard(bestMove);
		currPos = exportPosition();
		findNumPieces();
		status = -1;

	}
	else {
		if (checkMeter == 0) { // draw

			currPos = exportPosition();
			status = 0;
		}
		else { // checkMate
			status = 1;
		}
	}
	}

