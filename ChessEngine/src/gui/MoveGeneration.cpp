#include "Board.hpp"

/*
fills in the number of squares to the edge of the board for all directions for every square on the board. called at the beginning of the program
*/
void Board::fillNumToEdge(){
    	for (int i = 0; i < 64; i++) {
		int rank = i / 8;
		int file = i % 8;

		for (int j = 0; j < 8; j++) {
			switch (j) {
			case 0: numToEdge[i][j] = 7 - file; //right
				break;
			case 1: numToEdge[i][j] = file; //left
				break;
			case 2: numToEdge[i][j] = rank; //up
				break;
			case 3: numToEdge[i][j] = 7 - rank; //down
				break;
			case 4: numToEdge[i][j] = min(7 - rank, 7 - file); //bottomRight
				break;
			case 5: numToEdge[i][j] = min(7 - rank, file); //bottomLeft
				break;
			case 6: numToEdge[i][j] = min(rank, 7 - file); //topRight
				break;
			case 7: numToEdge[i][j] = min(rank, file); //topLeft
				break;
			}
		}
	}

}

/*
used for checking if a square is a viable square for the king to move to while in check
*/
bool Board::checkKingSquares(int inputSquare, int color){
	if (color) {
		if (squares[inputSquare] == 0 && blackTargets.find(inputSquare) == blackTargets.end()) {
			return true;
		}
	}
	else {
		if (squares[inputSquare] == 0 && whiteTargets.find(inputSquare) == whiteTargets.end()) {
			return true;
		}
	}
	return false;
}

/*
checks if a current square is being targeted by the opposite color
*/
bool Board::checkTargetByColor(int inputSquare, int color){
    	if (color) {
		if (blackTargets.find(inputSquare) == blackTargets.end()) {
			return true;
		}
	}
	else {
		if (whiteTargets.find(inputSquare) == whiteTargets.end()) {
			return true;
		}
	}
	return false;
}

/*
finds all the legal moves for the current position
*/
vector<string> Board::findLegalMoves(){
    numberWhiteMoves = 0;
	numberBlackMoves = 0;
	legalMoves.clear();
	findTargets();

	for (int i = 0; i < 64; ++i) {
		if (squares[i] != 0) {
			int pieceColor = determinePieceColor(i);
			if (pieceColor == isWhiteTurn) {
				switch (squares[i] & 0x7) {
				case 1: findKingMoves(i);
					break;
				case 2: findPawnMoves(i);
					break;
				case 3: findKnightMoves(i);
					break;
				case 4: findSlidingMoves(i);
					break;
				case 5: findSlidingMoves(i);
					break;
				case 6: findSlidingMoves(i);
					break;
				}
			}

		}
	}

	wasPinnedSquares = pinnedSquares;
	return legalMoves;
}

/*
finds all the legal moves for the sliding pieces (Queen, Rook, Bishop)
*/
void Board::findSlidingMoves(int startSquare){
    int startDirection = ((squares[startSquare] & 0x07) == 4) ? 4 : 0;
	int endDirection = ((squares[startSquare] & 0x07) == 5) ? 4 : 8;
	int isPinned = (pinnedSquares.find(startSquare) != pinnedSquares.end()) ? 1 : 0;
	for (int directionIndex = startDirection; directionIndex < endDirection; directionIndex++) {
		for (int i = 0; i < numToEdge[startSquare][directionIndex]; i++) {
			int targetSquare = startSquare + (directionOffsets[directionIndex] * (i + 1));
			if ((squares[targetSquare] & 0x7) == 0) {
				if (!isPinned || pinSquaresNew[startSquare].find(targetSquare) != pinSquaresNew[startSquare].end()) {
					addMove(startSquare, targetSquare);
				}
			}
			else if (determinePieceColor(startSquare) == determinePieceColor(targetSquare)) {
				break;
			}
			else {
				if (!isPinned || pinSquaresNew[startSquare].find(targetSquare) != pinSquaresNew[startSquare].end()) {
					addMove(startSquare, targetSquare);
				}
				break;
			}
		}
	}
    }
/*
finds all the legal pawn moves
*/
void Board::findPawnMoves(int startSquare){
    int pawnDir[3];
	int pieceColor = determinePieceColor(startSquare);
	int file = startSquare % 8;
	int rank = startSquare / 8;
	int passantLeft;
	int passantRight;
	int leftEdge;
	int rightEdge;
	int isPinned = (pinnedSquares.find(startSquare) != pinnedSquares.end()) ? 1 : 0;
	int wasPinned = (wasPinnedSquares.find(startSquare) != wasPinnedSquares.end()) ? 1 : 0;
	int doublePush;

	if (pieceColor) {
		pawnDir[0] = -8;
		pawnDir[1] = -9;
		pawnDir[2] = -7;
		passantLeft = (enPassantSquare == startSquare - 1 && !(!isPinned && wasPinned)) ? 1 : 0;
		passantRight = (enPassantSquare == startSquare + 1 && !(!isPinned && wasPinned)) ? 1 : 0;
		leftEdge = (file == 0) ? 1 : 0;
		rightEdge = (file == 7) ? 1 : 0;
		doublePush = (rank == 6) ? 1 : 0;
	}
	else {
		pawnDir[0] = 8;
		pawnDir[1] = 9;
		pawnDir[2] = 7;
		passantLeft = (enPassantSquare == startSquare + 1 && !(!isPinned && wasPinned)) ? 1 : 0;
		passantRight = (enPassantSquare == startSquare - 1 && !(!isPinned && wasPinned)) ? 1 : 0;
		leftEdge = (file == 7) ? 1 : 0;
		rightEdge = (file == 0) ? 1 : 0;
		doublePush = (rank == 1) ? 1 : 0;
	}
	int forward = startSquare + pawnDir[0];
	int left = startSquare + pawnDir[1];
	int right = startSquare + pawnDir[2];
	int twoPush = startSquare + pawnDir[0] * 2;

	//pawn push forward
	if (forward < 64 && forward > 0 && squares[forward] == 0) {
		if (!isPinned || pinSquaresNew[startSquare].find(forward) != pinSquaresNew[startSquare].end())
			addMove(startSquare, forward);
		if (doublePush && squares[twoPush] == 0) {
			if (!isPinned || pinSquaresNew[startSquare].find(twoPush) != pinSquaresNew[startSquare].end())
				addMove(startSquare, twoPush);
		}
	}


	if (left < 64 && left > 0 && (squares[left] != 0 || passantLeft) && determinePieceColor(startSquare) != determinePieceColor(left)) {
		if ((!isPinned || pinSquaresNew[startSquare].find(left) != pinSquaresNew[startSquare].end()) && !leftEdge)
			addMove(startSquare, left);
	}

	if (right > 0 && right < 64 && (squares[right] != 0 || passantRight) && determinePieceColor(startSquare) != determinePieceColor(right)) {
		if ( (!isPinned || pinSquaresNew[startSquare].find(right) != pinSquaresNew[startSquare].end())&& !rightEdge)
			addMove(startSquare, right);
	}

    }


/*
finds all of the legal knight moves
*/
void Board::findKnightMoves(int startSquare){
    int isPinned = (pinnedSquares.find(startSquare) != pinnedSquares.end()) ? 1 : 0;

	for (int i = 8; i < 16; i++) {
		int targetSquare = startSquare + directionOffsets[i];
		if (targetSquare >= 0 && targetSquare < 64 && determinePieceColor(startSquare) != determinePieceColor(targetSquare)) {
			int pieceFile = startSquare % 8;
			int targetFile = (targetSquare) % 8;
			if (abs(pieceFile - targetFile) <= 2) {
				if (!isPinned || pinSquaresNew[startSquare].find(targetSquare) != pinSquaresNew[startSquare].end()) {
					addMove(startSquare, targetSquare);
				}
			}
		}
	}
    }

/*
finds all the legal king moves
*/
void Board::findKingMoves(int startSquare){
        int color = determinePieceColor(startSquare);
	int kingSideRights = (color) ? rights[WHITEKINGRIGHTS] : rights[BLACKKINGRIGHTS];
	int queenSideRights = (color) ? rights[WHITEQUEENRIGHTS] : rights[BLACKQUEENRIGHTS];
	if (kingSideRights && startSquare % 8 == 4 && checkKingSquares(startSquare + 1, color) && checkKingSquares(startSquare + 2, color)) {
		if ((squares[startSquare+3]&0x7) == 5)
		addMove(startSquare, startSquare + 2);
	}

	if (queenSideRights && startSquare % 8 == 4 && checkKingSquares(startSquare - 1, color) && checkKingSquares(startSquare - 2, color) && checkKingSquares(startSquare - 3, color)) {
		if ((squares[startSquare + 3] & 0x7) == 5)
		addMove(startSquare, startSquare - 2);
	}

	for (int i = 0; i < 8; ++i) {
		int targetSquare = startSquare + directionOffsets[i];
		if (targetSquare >= 0 && targetSquare < 64 && checkTargetByColor(targetSquare, color) && determinePieceColor(targetSquare) != color
			&& numToEdge[startSquare][i] > 0) {
			addMove(startSquare, targetSquare);
		}
	}
    }
/*
adds a move to the set of legal moves based on the input start square and the input target square
*/
void Board::addMove(int startSquare, int targetSquare){
        	int pieceType = squares[startSquare] & 0x7;
	int pieceColor = determinePieceColor(startSquare);
	int startFile = startSquare % 8;
	int startRank = 8 - (startSquare / 8) ;
	int targetFile = targetSquare % 8;
	int targetRank = 8 - (targetSquare / 8);
	std::string move = fileToLetter(startFile) + std::to_string(startRank) + fileToLetter(targetFile) + std::to_string(targetRank);

	if (checkMoveHandler(targetSquare) || checkMeter == 0 || pieceType == 1) {
		if (pieceColor) {
			numberWhiteMoves++;
		}
		else {
			numberBlackMoves++;
		}
		if ((targetSquare < 8 || targetSquare > 55) && pieceType == 2) { //promotions

			legalMoves.push_back(move + "q");
			legalMoves.push_back(move + "r");
			legalMoves.push_back(move + "b");
			legalMoves.push_back(move + "n");
		}
		else { // normal move
			legalMoves.push_back(move);
		}

	}

    }


/*
converts an int representing the file of a piece and converts it into the corresponding letter used in chess notation
*/
string Board::fileToLetter(int file){
        	switch (file) {
	case 0:return "a";
		break;
	case 1:return "b";
		break;
	case 2: return "c";
		break;
	case 3: return "d";
		break;
	case 4: return "e";
		break;
	case 5: return "f";
		break;
	case 6: return "g";
		break;
	case 7: return "h";
		break;
	default: return "z";
		break;
	}

    }

/*
function that handles check position
*/
bool Board::checkMoveHandler(int targetSquare){
        	if (checkSquares.find(targetSquare) != checkSquares.end() && checkCounter <= 1) {
		return true;
	}
	else {
		return false;
	}
    }


/*
finds all of the targets of the opposite color, also determines if the position is a check position and the pinned pieces, used to determine all of the legal moves
*/
void Board::findTargets(){
    whiteTargets.clear();
	blackTargets.clear();
	checkSquares.clear();
	pinnedSquares.clear();
	wasPinnedSquares.clear();
	pinSquaresNew.clear();
	pawnFoundTargets.clear();

	checkMeter = 0;
	checkCounter = 0;
	for (int i = 0; i < 64; i++) {
		int temp = squares[i];
		if (squares[i] != 0) {
			int pieceColor = determinePieceColor(i);
			if (pieceColor != isWhiteTurn) {	
				switch (squares[i] & 0x7) {
				case 1: kingTargets(i);
					break;
				case 2: pawnTargets(i);
					break;
				case 3: knightTargets(i);
					break;
				case 4: slidingTargets(i);
					break;
				case 5: slidingTargets(i);
					break;
				case 6: slidingTargets(i);
					break;
				default: 
					break;
				}
			}
		}
	}

    }

/*
finds all the targets of sliding pieces, this function also finds all of the pinned pieces since only sliding pieces can pin other pieces
*/
void Board::slidingTargets(int startSquare){
        	vector<int>checkedSquares;
	int pinFlag = 0;
	int startDirection = ((squares[startSquare] & 0x07) == 4) ? 4 : 0;
	int endDirection = ((squares[startSquare] & 0x07) == 5) ? 4 : 8;

	for (int directionIndex = startDirection; directionIndex < endDirection; directionIndex++) {
		checkedSquares.clear();
		pinFlag = 0;
		int possiblePin = 0;
		for (int i = 0; i < numToEdge[startSquare][directionIndex]; i++) {
			int targetSquare = startSquare + (directionOffsets[directionIndex] * (i + 1));
			int pieceType = squares[targetSquare] & 0x7;
			if (pinFlag) {
				if (pieceType != 0 && pieceType != 1) {
					break;
				}
				else if (pieceType == 1 && determinePieceColor(startSquare) != determinePieceColor(targetSquare)) {
					pinnedSquares.insert(possiblePin);
					for (int l = 0; l < checkedSquares.size(); l++) {
						pinSquaresNew[possiblePin].emplace(checkedSquares[l]);
					}
					pinSquaresNew[possiblePin].emplace(startSquare);
					break;
				}
				else {
					checkedSquares.push_back(targetSquare);
				}
			}
			else if (squares[targetSquare] == 0) {
				checkedSquares.push_back(targetSquare);
				addTarget(startSquare, targetSquare);

			}
			else if (determinePieceColor(startSquare) == determinePieceColor(targetSquare)) {
				if (pieceType != 1) {
					addTarget(startSquare, targetSquare);
				}
				break;
			}
			else {
				addTarget(startSquare, targetSquare);
				if (pieceType == 1) {
					for (int j = 0; j < checkedSquares.size(); j++) {
						checkSquares.insert(checkedSquares[j]);
					}
				}
				else {
					possiblePin = targetSquare;
					pinFlag = 1;
				}
			}
		}

	}


    }

/*
finds all the targets of pawns
*/
void Board::pawnTargets(int startSquare){
        	int color = determinePieceColor(startSquare);
	int rank = startSquare / 8;
	int file = startSquare % 8;
	if (color) {
		if (startSquare - 7 > 0 && file != 7 && !((squares[startSquare - 7] & 0x7) == 1 && color == determinePieceColor(startSquare - 7))) {
			addTarget(startSquare, startSquare - 7);
			pawnFoundTargets.insert(startSquare - 7);
		}
		if (startSquare - 9 > 0 && file != 0 && !((squares[startSquare - 9] & 0x7) == 1 && color == determinePieceColor(startSquare - 9))) {
			addTarget(startSquare, startSquare - 9);
			pawnFoundTargets.insert(startSquare - 9);
		}

	}
	else {
		if (startSquare + 7 < 64 && file != 0 && !((squares[startSquare + 7] & 0x7) == 1 && color == determinePieceColor(startSquare + 7))) {
			addTarget(startSquare, startSquare + 7);
			pawnFoundTargets.insert(startSquare + 7);
		}
		if (startSquare + 9 < 64 && file != 7 && !((squares[startSquare + 9] & 0x7) == 1 && color == determinePieceColor(startSquare + 9))) {
			addTarget(startSquare, startSquare + 9);
			pawnFoundTargets.insert(startSquare + 9);
		}
	}
    }

/*
finds all the targets of knights
*/
void Board::knightTargets(int startSquare){
        	int file = startSquare % 8;
	int rank = startSquare / 8;

	for (int i = 8; i < 16; ++i) {
		int targetSquare = startSquare + directionOffsets[i];
		int targetFile = targetSquare % 8;
		int targetRank = targetSquare / 8;
		if (abs(file - targetFile) <= 2 && targetSquare >= 0 && targetSquare < 64
			&& !(((squares[targetSquare] & 0x7) == 1) && determinePieceColor(startSquare) == determinePieceColor(targetSquare))) {
			addTarget(startSquare, targetSquare);
		}

	}

    }

/*
finds all the targets of the king
*/
void Board::kingTargets(int startSquare){
        	for (int i = 0; i < 8; ++i) {
		int targetSquare = startSquare + directionOffsets[i];
		if (targetSquare >= 0 && targetSquare < 64) {
			addTarget(startSquare, targetSquare);
		}
	}

    }

/*
adds a found target to the set of targets based on the start square and the target square
*/
void Board::addTarget(int startSquare, int targetSquare){
        	if ((squares[targetSquare] & 0x7) == 1) {
		checkCounter++;
		checkMeter = (determinePieceColor(startSquare)) ? 2 : 1;
		checkSquares.insert(startSquare);
	}
	if (determinePieceColor(startSquare) == 1) {
		whiteTargets.insert(targetSquare);
	}
	else {
		blackTargets.insert(targetSquare);
	}

    }

/*
determines the current number of pieces based of the piece counts array
*/
void Board::findNumPieces(){
        	int myNumPieces = 0;
	for (int i = 0; i < 10; i++) {
		myNumPieces += pieceCounts[i];
	}
	numPieces = myNumPieces;
    }