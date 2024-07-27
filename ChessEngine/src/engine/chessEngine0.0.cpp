#include "chessEngine0.0.hpp"

/*
Very simple evaluation function, simply counts material and returns the currcolor - oppositecolor result
*/
	int Engine00::evaluatePos() {
		int currValuation = 0;
		int perspective = (isWhiteTurn) ? 1 : -1;
		currValuation = initValues() * perspective;

		return currValuation;
	}


/*
This is a naive minimax search algorithm that determines the bestmove at a certain depth by checking every possible position at that depth.
*/

	int Engine00::search(int depth,int rootDepth, string& bestMove) {
		int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights};
		int currSquares[64];
		int currPieceCount[10];
		memcpy(currSquares, squares, sizeof(int)*64);
		memcpy(currPieceCount, pieceCounts, sizeof(int) * 10);

		if (depth == 0) {
			return evaluatePos();
		}
		vector<string> moves = findLegalMoves();
		if (moves.size() == 0) {
				if (checkMeter != 0) {
				return INT32_MIN + (rootDepth - depth);
			}
			return 0;
		}
		string bestFoundMove;
		int bestEvaluation = INT32_MIN;


		for (auto element : moves) {
			totalMoves++;
			updateBoard(element);
			int evaluation = -search(depth - 1 , rootDepth, bestMove);
		undoMove(currSquares, currPieceCount, currRights);
		bestEvaluation = updateMove(element, bestFoundMove, bestEvaluation, evaluation);
		}

		if (rootDepth == depth) {
			bestMove = bestFoundMove;
		}

		return bestEvaluation;
	}


/*This function undos whatever move was played by taking the inputs of the state of the board before the move was played and returning the board to that state.*/
	void Engine00::undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]) { // swaps the array that contains what the old position was with the updated position
		memcpy(squares, currSquares, sizeof(int)*64);
		memcpy(pieceCounts, currPieceCount, sizeof(int)*10);
		whiteKingRights = currRights[0];
		whiteQueenRights = currRights[1];
		blackKingRights = currRights[2];
		blackQueenRights = currRights[3];
		isWhiteTurn ^= 1;
	}

/*
This function counts material for white - black
*/
	int Engine00::initValues() {
		int totalValue = 0;

		totalValue += (pieceCounts[0] * 100) - (pieceCounts[5] * 100);
		totalValue += (pieceCounts[1] * 300) - (pieceCounts[6] * 300);
		totalValue += (pieceCounts[2] * 300) - (pieceCounts[7] * 300);
		totalValue += (pieceCounts[3] * 500) - (pieceCounts[8] * 500);
		totalValue += (pieceCounts[4] * 900) - (pieceCounts[9] * 900);
		return totalValue;
	}

/*
This function simply updates the bestmove that is found with the current move if its evaluation is greater, otherwise maintains the same value
*/
	int Engine00::updateMove(string& currMove,string& bestFoundMove, int& bestEval, int& currEval) {
		if (currEval > bestEval) {
			bestFoundMove = currMove;
			return currEval;
		}
		else {
			return bestEval;
		}
	}

/*
This function is called to search for the best move at the current position and updates the object with the current status of the game (is checkmate)
and what the best computed move by the engine is
*/
	void Engine00::handleGo(const string& input){
		int depth = 4;
		centiPawn = search(depth , depth, bestMove);
		string currPosition = exportPosition();
		legalMoves = findLegalMoves();
		if (legalMoves.size() > 0) {
			updateBoard(bestMove);
			currPos = exportPosition();
			findNumPieces();
			status = -1;
		}
		else if (checkMeter == 0) {
			currPos = exportPosition();
			status = 0;
		}
		else {
			currPos = exportPosition();
			status = 1;
			
		}
	/*	cout << "Current centiPawn: " << centiPawn << endl;
		cout << "Total Moves Played: " << totalMoves << endl;
		cout << "Best Move : " << bestMove << endl;
		*/
}

