#include "Engine0.1.hpp"

/*
I am going to attempt to use this object to create a multithreaded engine. My strategy is to have this object take moves as input to set it up
and then let it fly on the moves given to it. This way i can create multiple of these objects and have each thread work on one object
*/
	int Engine01::searchRoot(int depth, int& rootDepth, string& rootMove, vector<string>& givenMoves) {
		int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights };
		int currSquares[64];
		int currPieceCount[10];
		memcpy(currSquares, squares, sizeof(int) * 64);
		memcpy(currPieceCount, pieceCounts, sizeof(int) * 10);

		if (depth == 0) {
			return evaluatePos();
		}

		if (givenMoves.size() == 0) {
			if (checkMeter != 0) {
				return INT32_MIN + (rootDepth - depth);
			}
			return 0;
		}

		int bestEval = INT32_MIN;
		string bestFoundMove = "";

		for (auto currMove : givenMoves) {
			totalMoves++;
			updateBoard(currMove);
			int evaluation = -search(depth - 1, rootDepth, bestFoundMove);
			undoMove(currSquares, currPieceCount, currRights);
			if (evaluation > bestEval) {
				bestEval = evaluation;
				rootMove = currMove;
			}
		}
		return bestEval;
	}


/*
Main search function. Keeps digging into each move until it reaches the end depth and returns the evaluation at the final depth. Check every possible position
*/
	int Engine01::search(int depth, int& rootDepth, string& rootMove) {
		int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights };
		int currSquares[64];
		int currPieceCount[10];
		memcpy(currSquares, squares, sizeof(int) * 64);
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
			int evaluation = -search(depth - 1, rootDepth, bestMove);
			undoMove(currSquares, currPieceCount, currRights);
			bestEvaluation = max(bestEvaluation, evaluation);
		//	bestEvaluation = updateMove(element, bestFoundMove, bestEvaluation, evaluation);
		}

	/*	if (rootDepth == depth) {
			bestMove = bestFoundMove;
		}*/

		return bestEvaluation;

	}

/*
Evaluates the current position. Simply counts material and returns which color has more material and by how much
*/
	int Engine01::evaluatePos() {
		int currValuation = 0;
		int perspective = (isWhiteTurn) ? 1 : -1;
		currValuation = initValues() * perspective;

		return currValuation;
	}


//undos the move that was played prior for reverting the board state
	void Engine01::undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]) { // swaps the array that contains what the old position was with the updated position
		memcpy(squares, currSquares, sizeof(int) * 64);
		memcpy(pieceCounts, currPieceCount, sizeof(int) * 10);
		whiteKingRights = currRights[0];
		whiteQueenRights = currRights[1];
		blackKingRights = currRights[2];
		blackQueenRights = currRights[3];
		isWhiteTurn ^= 1;
	}


// finds the raw material value of all of the chess pieces left on the board
	int Engine01::initValues() {
		int totalValue = 0;

		totalValue += (pieceCounts[0] * 100) - (pieceCounts[5] * 100);
		totalValue += (pieceCounts[1] * 300) - (pieceCounts[6] * 300);
		totalValue += (pieceCounts[2] * 300) - (pieceCounts[7] * 300);
		totalValue += (pieceCounts[3] * 500) - (pieceCounts[8] * 500);
		totalValue += (pieceCounts[4] * 900) - (pieceCounts[9] * 900);
		return totalValue;
	}

//updates the best move if the current move has a greater evaluation
	int Engine01::updateMove(string& currMove, string& bestFoundMove, int& bestEval, int& currEval) {
		if (currEval > bestEval) {
			bestFoundMove = currMove;
			return currEval;
		}
		else {
			return bestEval;
		}
	}

//starts the search for the best move
	void Engine01::handleGo(const string& input) {
		int depth = 4;
		endBestEval = searchRoot(depth, depth, endBestMove, givenMoves);
	}

//factory function used to create a new engine01 mainly for multi threading
	Engine* Engine01::engineCreator(string position, vector<string> startMoves,int* globalAlpha, mutex* globalAlphaMutex,int* globalBeta, mutex* globalBetaMutex) {
		return  new Engine01(position, startMoves);
	}


//constructor
	Engine01::Engine01(string position, const vector<string>& startMoves) {
		handlePosition("position" + position);
		givenMoves = startMoves;
	}

//lets outside code obtain the current best evaluation
	int Engine01::getBestEval() {
		return endBestEval;
	}
//lets outside code obtain the current best move
	string Engine01::getBestMove() {
		return endBestMove;
	}
//lets outside code obtain the best pair of the evaluation and move
	pair<int, string> Engine01::getBestPair() {
		 pair<int, string> returnPair(endBestEval, endBestMove);
		 return returnPair;
	}
//obtains the total positions searched by the engine when determining the best move
	int Engine01::getTotalMoves(){
		return totalMoves;
	}
