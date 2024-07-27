
#include "Engine02.hpp"

using namespace std;
using std::lower_bound;

/*
Root of the search function. Starts the recursive search function call and determines the best move found out of the regualr search function
*/
int Engine02::searchRoot(int depth, int& rootDepth, string& rootMove, vector<string>& givenMoves) {
	int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights };
	int currSquares[64];
	int currPieceCount[10];
	memcpy(currSquares, squares, sizeof(int) * 64);
	memcpy(currPieceCount, pieceCounts, sizeof(int) * 10);

	if (givenMoves.empty()) {
		return checkMeter != 0 ? INT32_MIN : 0;
	}
	int bestEvaluation = INT32_MIN + 1;
	for (const auto& move : givenMoves) {
		totalMoves++;
		updateBoard(move);
		int eval = -search(INT32_MIN + 1, INT32_MAX - 1, depth - 1, rootDepth, isWhiteTurn);
		undoMove(currSquares, currPieceCount, currRights);
		if (eval > bestEvaluation) {
			bestEvaluation = eval;
			rootMove = move;
		}

		if (globalAlpha != nullptr){
			if (eval > *globalAlpha){
				lock_guard<mutex> lock(*globalAlphaMutex);
				*globalAlpha = eval;
			}
		}
	}

	return bestEvaluation;
}


/*
main search function where the bulk of the work is done. Continues searching until a given depth recursively, pruning inferior branches based
on the alpha beta search algorithm
*/
int Engine02::search(int alpha, int beta, int depthLeft, int& rootDepth, int initColor) {
	int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights };
	int currSquares[64];
	int currPieceCount[10];
	memcpy(currSquares, squares, sizeof(int) * 64);
	memcpy(currPieceCount, pieceCounts, sizeof(int) * 10);
	if (depthLeft == 0) {
		return quiesceSearch(alpha,beta,initColor, depthLeft, rootDepth);
	}
	vector<string> moves = findLegalMoves();

	if (moves.empty()) {
		return (checkMeter != 0) ? INT32_MIN + (rootDepth - depthLeft) : 0;
}

	int bestEval = INT32_MIN + 1;
	sortMoves(moves);
	for (const auto& move : moves) {
		totalMoves++;
		updateBoard(move);
		int eval = -search(-beta, -alpha, depthLeft - 1, rootDepth, initColor);
		undoMove(currSquares, currPieceCount, currRights);	
		if (eval >= beta)
			return eval;
		if (eval > bestEval) {
			bestEval = eval;
			alpha = max(eval, alpha);
		}
		if (globalAlphaBreak(initColor,eval,depthLeft,rootDepth)){
			return eval;
		}
	}
	return bestEval;
}

/*
This is a special search function that only evaluates capture moves. This function helps to overcome the "horizon effect" where at the final depth one side 
can take say a pawn with their queen and they are up material, ignoring the opponents next move where they win the queen.
*/
int Engine02::quiesceSearch(int alpha, int beta, int& initColor,int& depthLeft, int& rootDepth) {
	int standPat = evaluatePos();
	int currRights[4] = { whiteKingRights, whiteQueenRights, blackKingRights, blackQueenRights };
	int currSquares[64];
	int currPieceCount[10];
	memcpy(currSquares, squares, sizeof(int) * 64);
	memcpy(currPieceCount, pieceCounts, sizeof(int) * 10);
	if (standPat >= beta) {
		return beta;
	}
	alpha = max(alpha, standPat);
	vector<string> moves = findCaptureMoves();
	sortMoves(moves);

	for (const auto& move: moves) {
			totalMoves++;
			updateBoard(move);
			standPat = -quiesceSearch(-beta, -alpha, initColor,depthLeft,rootDepth);
			undoMove(currSquares, currPieceCount, currRights);
			if (standPat >= beta)
				return beta;
			alpha = max(alpha, standPat);

			if (globalAlphaBreak(initColor,standPat,depthLeft,rootDepth)){
				return standPat;
			}
	}


	return alpha;
}

/*
This is a special function that may not even work correctly. Was intended to try to prune some moves between different threads by using the best evaluation found globally 
between the different threads. Had lots of trouble getting this to sort of work.
*/
bool Engine02::globalAlphaBreak(int& initColor, int& eval, int& depthLeft, int& rootDepth) {
	if (globalAlpha != nullptr) {
		if (initColor == isWhiteTurn) {
			if (eval > -*globalAlpha && depthLeft == rootDepth - 1) { // opponent turn
				return true;
			}
		}
	}
	return false;
}

/*
determines if a certain move is a capture
*/
bool Engine02::isCapture(string move) {
	int startSquare = stringToSquare(move.substr(0, 2));
	int targetSquare = stringToSquare(move.substr(2, 2));
	int startPieceType = squares[startSquare] & 0x7;
	int targetPieceType = squares[targetSquare] & 0x7;

	if (startPieceType == 2) {
		if (abs(startSquare - targetSquare) % 8 != 0) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		if (targetPieceType != 0) {
			return true;
		}
		else {
			return false;
		}
	}
}


/*
finds all of the capture moves for the quiesce search
*/
vector<string> Engine02::findCaptureMoves(){
	vector<string> capturingMoves;
	vector<string> moves = findLegalMoves();

	for (const auto& move : moves){
		if (isCapture(move)) {
			capturingMoves.push_back(move);
		}
	}
		return capturingMoves;
}


/*
simple evaluation function returns current material imbalance
*/
int Engine02::evaluatePos() {
	int currValuation = 0;
	int perspective = (isWhiteTurn) ? 1 : -1;
	currValuation = initValues() * perspective;

	return currValuation;
}


/*
undos the last move that was played
*/
void Engine02::undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]) {
	memcpy(squares, currSquares, sizeof(int) * 64);
	memcpy(pieceCounts, currPieceCount, sizeof(int) * 10);
	whiteKingRights = currRights[0];
	whiteQueenRights = currRights[1];
	blackKingRights = currRights[2];
	blackQueenRights = currRights[3];
	isWhiteTurn ^= 1;
}


/*
finds the material imbalence
*/
int Engine02::initValues() {
	int totalValue = 0;

	totalValue += (pieceCounts[0] * 100) - (pieceCounts[5] * 100);
	totalValue += (pieceCounts[1] * 300) - (pieceCounts[6] * 300);
	totalValue += (pieceCounts[2] * 300) - (pieceCounts[7] * 300);
	totalValue += (pieceCounts[3] * 500) - (pieceCounts[8] * 500);
	totalValue += (pieceCounts[4] * 900) - (pieceCounts[9] * 900);
	return totalValue;
}


/*
updates the best found move if the current move has a greater evaluation than it
*/
int Engine02::updateMove(string& currMove, string& bestFoundMove, int& bestEval, int& currEval) {
	if (currEval > bestEval) {
		bestFoundMove = currMove;
		return currEval;
	}
	else {
		return bestEval;
	}
}

/*
starts the search for the best move at the current position
*/
void Engine02::handleGo(const string& input) {
	int depth = 4;
	endBestEval = searchRoot(depth, depth, endBestMove, givenMoves);
/*	cout << "Best Eval: " << endBestEval << endl;
	cout << "Best Move: " << endBestMove << endl;
	cout << "Total Moves Searched: " << totalMoves << endl;
	*/
}


/*
Sorting algorithm to help increase the effectiveness of the alpha beta algorithm
*/
 void Engine02::sortMoves(vector<string>& moves) {
	vector<pair<int, string>> vectorPair;

	for (auto currMove : moves) {
		assignValue2(vectorPair, currMove);
	}

	moves.clear();
	for (int i = 0; i < vectorPair.size(); ++i) {
		moves.push_back(vectorPair[i].second);
	}
}


/*
meant to be a more effective and more complicated function to guess the value of different moves before seaching,
currently less effective than the simpler function so currently not in use
*/
void Engine02::assignValue(vector<pair<int, string>>& givenVec, string move) {
	int startSquare = stringToSquare(move.substr(0, 2));
	int targetSquare = stringToSquare(move.substr(2, 2));
	int startPieceType = squares[startSquare] & 0x7;
	int targetPieceType = squares[targetSquare] & 0x7;

	
	switch (startPieceType) {
	case 1: kingMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	case 2:pawnMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	case 3:knightMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	case 4:bishopMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	case 5:rookMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	case 6:queenMoveValue(givenVec, startSquare, targetSquare, targetPieceType, move);
		break;
	}
}

/*
simple function to assign the value of a particualr move. Particularly likes captures with pawns, and promotions
*/
void Engine02::assignValue2(vector<pair<int, string>>& givenVec, string move) {
	int startSquare = stringToSquare(move.substr(0, 2));
	int targetSquare = stringToSquare(move.substr(2, 2));
	int startPieceType = squares[startSquare] & 0x7;
	int targetPieceType = squares[targetSquare] & 0x7;

	pair<int, string> tempPair(0, move);
	 if (startPieceType != 1 && targetPieceType != 0) {
		tempPair.first = 10*(pieceTypeToValue(targetPieceType) - pieceTypeToValue(startPieceType));
	}

	 if (startPieceType == 2 && (targetSquare / 8 == 0 || targetSquare / 8 == 7)) {
		 tempPair.first += pawnToPromotion(move.back());
	 }

	 if (pawnFoundTargets.find(targetSquare) != pawnFoundTargets.end()) {
		 tempPair.first -= pieceTypeToValue(startPieceType);
	}
	insertSortVec(givenVec, tempPair);
}

//returns the value of a pawn promotion
int Engine02::pawnToPromotion(char promoteType) {
	switch (promoteType) {
	case 'q': return 900;
		break;
	case 'r': return 500;
		break;
	case 'b': return 320;
		break;
	case 'n': return 300;
		break;
	default: return 0;
		break;
	}
}

// determines the guess of a value of a king move
void Engine02::kingMoveValue(vector<pair<int,string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	int startFile = startSquare % 8;
	int targetFile = targetSquare % 8;
	int startRank = startSquare / 8;
	int targetRank = targetSquare / 8;
	int pieceColor = determinePieceColor(startSquare);
	pair<int, string> insertPair(0, move);
	if (abs(startFile - targetFile) > 1) { // give higher priority to castling
		insertPair.first = 100;
	}
	else if (numPieces <= 10) { // give some higher priority to king moves in endgame
		if (targetPieceType != 0) { // prefer captures in endgame
			insertPair.first = pieceTypeToValue(targetPieceType);
		}
		else {
			if (pieceColor) {
				insertPair.first = abs(startRank - 7) * 50;
			}
			else {
				insertPair.first = startRank * 50;
			}
		}
	}
	else { // normal move calculation
		if (targetPieceType != 0) { // prefer captures but not as much
			insertPair.first = pieceTypeToValue(targetPieceType);
		}
		else {
			insertPair.first = -100; // discourage moving the king otherwise
		}
	}

	insertSortVec(givenVec, insertPair);
}


//determines the guess of a value of a pawn move
void Engine02::pawnMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	int startRank = startSquare / 8;
	int pieceColor = determinePieceColor(startSquare);
	int adjustedRank = (pieceColor) ? abs(startRank - 7) : startRank;
	pair<int, string> insertPair(0, move);
	if (numPieces < 12) { // endgame
		if (adjustedRank == 6) { // heavily prefer promotions
			insertPair.first = pieceTypeToValue(charToPiece(move.back()) - ((pieceColor) ? 8 : 16)) + 200;
		}
		else {
				 if (targetPieceType != 0) {
				insertPair.first = (pieceTypeToValue(targetPieceType)+adjustedRank);
			}
			else {
				insertPair.first = adjustedRank * 50;
			}
		}
	}
	else {
		if (adjustedRank == 6) { // heavily prefer promotions
			insertPair.first = pieceTypeToValue(charToPiece(move.back()) - ((pieceColor) ? 8 : 16)) + 200;
		}
		else {
			 if (targetPieceType != 0) {
				insertPair.first = (pieceTypeToValue(targetPieceType) + adjustedRank);
			}
			else {
				insertPair.first = adjustedRank * 25;
			}
		}
	}
	insertSortVec(givenVec, insertPair);
}

// returns the guess of a value of a knight move
void Engine02::knightMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	pair<int, string> insertPair(0, move);
	if (targetPieceType != 0) {
		insertPair.first = pieceTypeToValue(targetPieceType) - 100;
	} 
	insertSortVec(givenVec, insertPair);
}

//returns the value of a guess of a bishop move
void Engine02::bishopMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	pair<int, string> insertPair(0, move);
	if (targetPieceType != 0) {
		insertPair.first = pieceTypeToValue(targetPieceType) - 300;
	}
	insertSortVec(givenVec, insertPair);
}

//returns the value of a guess of a rook move
void Engine02::rookMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	pair<int, string> insertPair(0, move);
	if (targetPieceType != 0) {
		insertPair.first = pieceTypeToValue(targetPieceType) - 500;
	}
	insertSortVec(givenVec, insertPair);
}


//returns the value of a guess of a queen move
void Engine02::queenMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move) {
	pair<int, string> insertPair(0, move);
	if (targetPieceType != 0) {
		insertPair.first = pieceTypeToValue(targetPieceType) - 900;
	}
	insertSortVec(givenVec, insertPair);
}



//now unused but was a compare function for a pair of int,string to use to sort the pairs in a vector
bool Engine02::compare(const pair<int, string>& a,const pair<int, string>& b) {
	return a.first > b.first;
}

//inserts a pair into a sorted vector of pairs correctly
void Engine02::insertSortVec(vector<pair<int, string>>& givenVec,const pair<int, string>& newPair) {
	    auto comp = [](const pair<int, string>& a, const pair<int, string>& b) {
        return a.first > b.first;
    };
	auto itr = lower_bound(givenVec.begin(), givenVec.end(),newPair, comp);
    givenVec.insert(itr, newPair);
}


//factory function for creating a new instance of this class. mainly for multithreading
Engine* Engine02::engineCreator(string position, vector<string> startMoves, int* globalAlpha, mutex* globalAlphaMutex,int* globalBetaGiven, mutex* globalBetaMutexGiven) {
	return new Engine02(position, startMoves,globalAlpha,globalAlphaMutex,globalBetaGiven,globalBetaMutexGiven);
}