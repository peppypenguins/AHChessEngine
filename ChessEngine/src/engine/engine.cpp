
#include "engine.hpp"
#include <intrin.h>
using namespace std;


/*executes the uci command*/
void Engine::handleUCI(const string& input) {
	std::cout << "id name AHChessEngine" << std::endl;
	std::cout << "id author AdamHerring" << std::endl;
	std::cout << "uciok" << std::endl;
}

/*executes the isReady command*/
void Engine::handleIsReady(const string& input) {
	std::cout << "readyok" << std::endl;
}

/*
This function updates the engine to be the same settings as the given FEN format string
*/
void Engine::handlePosition(const string& FEN) {
	int index = 0;
	while (index < FEN.size() && FEN[index] != ' ') {
		++index;
	}


	int file = 0;
	int rank = 0;

	memset(pieceCounts, 0, sizeof(int) * 10);
	memset(squares, 0, sizeof(int) * 64);

	whiteKingRights = 0;
	whiteQueenRights = 0;
	blackKingRights = 0;
	blackQueenRights = 0;






	for (int i = index + 1; i < FEN.size(); i++) {
		if (FEN[i] == '/') {
			file = 0;
			rank++;
		}
		else if (FEN[i] == ' ') {
			++i;
			isWhiteTurn = (FEN[i] == 'w') ? 1 : 0;

			while (i != FEN.size()) {
				switch (FEN[i]) {
				case 'k': blackKingRights = 1;
					break;
				case 'q': blackQueenRights = 1;
					break;
				case 'K': whiteKingRights = 1;
					break;
				case 'Q':whiteQueenRights = 1;
					break;
				}
				++i;
			}
		}
		else {
			if (std::isdigit(FEN[i])) {
				file += (FEN[i] - '0');
			}
			else {
				int pieceSquare = (rank * 8) + file;
				int squareValue = charToPiece(FEN[i]);
				squares[pieceSquare] = squareValue;
				++file;
			}
		}
	}
}

/*
this is a placeholder function that simply generates a random legal move. This should be replaced
by the virtual version of this function in a child object engine.
*/
void Engine::handleGo(const string& input) {
	findLegalMoves();

	vector<string> allMoves(legalMoves.begin(), legalMoves.end());
	if (allMoves.size() > 0) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distr(0, allMoves.size() - 1);
		int randomIndex = distr(gen);
		bestMove = allMoves[randomIndex];

		
	}
	else {
		bestMove = "a1a1";
	}
}


/*
executes the actual commands given to the engine
*/
void Engine::executeCmd(const string& input) {
	istringstream iss(input);
	string command;
	iss >> command;

	if (acceptedCommands.find(command) != acceptedCommands.end()) {
		int funcIndex = acceptedCommands.find(command)->second;
		(this->*uciCmds[funcIndex])(input);
	}
}


/*
converts a string that represents a chessmove to the starting square and target square of the move
*/
int Engine::stringToSquare(string inputString) {
	int rank = 0;
	int file = 0;
	switch (inputString[0]) {
	case 'a': file = 0;
		break;
	case 'b': file = 1;
		break;
	case 'c': file = 2;
		break;
	case 'd': file = 3;
		break;
	case 'e': file = 4;
		break;
	case 'f': file = 5;
		break;
	case 'g': file = 6;
		break;
	case 'h': file = 7;
		break;
	}
	rank =  8 - (inputString[1] - '0');

	return (rank * 8) + file;

}

/*
simply gives the int value for the given character
*/
int Engine::charToPiece(char inputChar) {
	switch (inputChar) {
	case 'r':pieceCounts[8]++;
		return 21;
		break;
	case 'n': pieceCounts[6]++;
		return 19;
		break;
	case 'b': pieceCounts[7]++;
		return 20;
		break;
	case 'q': pieceCounts[9]++;
		return 22;
		break;
	case 'k': 
		return 17;
		break;
	case 'p': pieceCounts[5]++;
		return 18;
		break;
	case 'R': pieceCounts[3]++;
		return 13;
		break;
	case 'N': pieceCounts[1]++;
		return 11;
		break;
	case 'B': pieceCounts[2]++;
		return 12;
		break;
	case 'Q': pieceCounts[4]++;
		return 14;
		break;
	case 'K': return 9;
		break;
	case 'P': pieceCounts[0]++;
		return 10;
		break;
	default: return -1;
		break;
	}
}


/*
Exports the current position of the engine as a FEN string to be used by other classes primarily. Especially the test engine file
*/
std::string Engine::exportPosition() {
	std::string returnString = " ";
	char moveTurn = (isWhiteTurn) ? 'w' : 'b';
	for (int i = 0; i < 8; i++) {
		int space = 1;
		for (int j = 0; j < 8; j++) {
			int currSquare = (i * 8) + j;
			int pieceColor = determinePieceColor(currSquare);
			if ((squares[currSquare] & 0x7) == 0) {
				if (std::isdigit(returnString.back()))
					returnString.back() = space + '0';
				else
					returnString += space + '0';
				space++;
			}
			else {
				char pieceChar = pieceToChar(squares[currSquare] & 0x7);
				pieceChar = (pieceColor) ? toupper(pieceChar) : pieceChar;
				returnString += pieceChar;
				space = 1;
			}
		}
		returnString += '/';
	}
	returnString.back() = ' ';
	returnString += moveTurn;
	returnString += ' ';
	if (whiteKingRights)
		returnString += 'K';
	if (whiteQueenRights)
		returnString += 'Q';
	if (blackKingRights)
		returnString += 'k';
	if (blackQueenRights)
		returnString += 'q';
	return returnString;
}


/*
Updates the internal board of the engine based off of a given string move that should be played. No checking for legality of said move, so
must make sure is a legal move
*/
void Engine::updateBoard(string move) {
	int startSquare = stringToSquare(move.substr(0, 2));
	int targetSquare = stringToSquare(move.substr(2, 2));
	int startFile = startSquare % 8;
	int targetFile = targetSquare % 8;
	int pieceType = squares[startSquare] & 0x7;
	int pieceArrayIndex = pieceCountsIndex(squares[startSquare]);
	int targetArrayIndex = pieceCountsIndex(squares[targetSquare]);
	int color = (determinePieceColor(startSquare)) ? 8 : 16;
	int promotionOffset = (determinePieceColor(startSquare)) ? 0 : 5;
	pieceMove = pieceType;
	if (pieceType == 1) {
		if (isWhiteTurn) {
			whiteKingRights = 0;
			whiteQueenRights = 0;
		}
		else {
			blackKingRights = 0;
			blackQueenRights = 0;
		}
	}

	if (pieceType == 5) {
		if (isWhiteTurn) {
			if (startFile < 4) {
				whiteQueenRights = 0;
			}
			else {
				whiteKingRights = 0;
			}
		}
		else {
			if (startFile < 4) {
				blackQueenRights = 0;
			}
			else {
				blackKingRights = 0;
			}

		}
	}

	if (pieceType == 1 && abs(startFile - targetFile) > 1) { // king castle
		int rookSquare;
		if (targetSquare > startSquare) { // kingside
			rookSquare = startSquare + 3;
			squares[targetSquare] = squares[startSquare];
			squares[startSquare] = 0;
			squares[startSquare + 1] = squares[rookSquare];
			squares[rookSquare] = 0;
		}
		else { // queenside
			rookSquare = startSquare - 4;
			squares[targetSquare] = squares[startSquare];
			squares[startSquare] = 0;
			squares[startSquare - 1] = squares[rookSquare];
			squares[rookSquare] = 0;

		}
	}
	else  if (pieceType == 2 && abs(startSquare - targetSquare) % 8 != 0 && squares[targetSquare] == 0) { // en passant
		pieceCounts[abs(promotionOffset - 5)]--; // remove 1 opposing pawn from count
		if (determinePieceColor(startSquare)) {
			if (startSquare - targetSquare > 8) {
				squares[startSquare - 1] = 0;
			}
			else {
				squares[startSquare + 1] = 0;
			}

		}
		else {
			if (abs(startSquare - targetSquare) > 8) {
				squares[startSquare + 1] = 0;
			}
			else {
				squares[startSquare - 1] = 0;
			}

		}
		squares[targetSquare] = squares[startSquare];
		squares[startSquare] = 0;
	} else {

		switch (move.back()) { // piece promotion
		case 'q':squares[targetSquare] = color | 6;
			pieceCounts[pieceArrayIndex]--;
			pieceCounts[4 + promotionOffset]++;
			break;
		case 'r':squares[targetSquare] = color | 5;
			pieceCounts[pieceArrayIndex]--;
			pieceCounts[3 + promotionOffset]++;
			break;
		case 'n':squares[targetSquare] = color | 3;
			pieceCounts[pieceArrayIndex]--;
			pieceCounts[1 + promotionOffset]++;
			break;
		case 'b': squares[targetSquare] = color | 4;
			pieceCounts[pieceArrayIndex]--;
			pieceCounts[2 + promotionOffset]++;
			break;
		default: if (squares[targetSquare] != 0) { pieceCounts[targetArrayIndex]--; }
			if ((squares[startSquare]&0x7) == 2 && (abs(startSquare-targetSquare) == 16)){
				enPassantSquare = targetSquare;
			} else {
				enPassantSquare = -1;
			}
			squares[targetSquare] = squares[startSquare]; // normal move
			break;
		}
		squares[startSquare] = 0;
	}
	isWhiteTurn = isWhiteTurn ^ 1;
}


/*
The number of each type of piece of each color is kept in an array attaached to this class, this function determines which index in that array
a given piece should be based on the value at that square.
*/
int Engine::pieceCountsIndex(int squareValue) {
	switch (squareValue) {
	case 10: return 0;
		break;
	case 11: return 1;
		break;
	case 12: return 2;
		break;
	case 13: return 3;
		break;
	case 14: return 4;
		break;
	case 18: return 5;
		break;
	case 19: return 6;
		break;
	case 20: return 7;
		break;
	case 21: return 8;
		break;
	case 22: return 9;
		break;
	default: return 0;
		break;
	}

}

/*
This function takes the internal piece values that are used throughout the project as input and outputs the character associated with that piece.
*/
char Engine::pieceToChar(int pieceType) {
	switch (pieceType) {
	case 1: return 'k';
		break;
	case 2: return 'p';
		break;
	case 3: return 'n';
		break;
	case 4: return 'b';
		break;
	case 5: return 'r';
		break;
	case 6: return 'q';
		break;
	default: return 'p';
		break;
	}
}


/*
This function determines the number of pieces still on the board using the piece counts array that stores the number of each type of piece 
for each color.
*/
void Engine::findNumPieces() {
	int myNumPieces = 0;
	for (int i = 0; i < 10; i++) {
		myNumPieces += pieceCounts[i];
	}
	numPieces = myNumPieces;
}


/*
This is the function that does most of the work for the chess engines by determining all of the legal moves at the given position. Does this 
by finding all of the targeted squares of the opponent, and all of the current pinned squares. As well as some data from previous moves concerning pawns
to determine what all of the legal moves should be
*/
vector<string> Engine::findLegalMoves() {
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
No longer used function. At one point was used to attempt to multi thread finding the different legal moves but the generation of multiple threads was more
costly than the benefit they provided on such a small amount of work to do.
*/
void Engine::processArrayPart(int start, int end) {
	for (int i = start; i < end; ++i) {
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



}

/*
This function finds all the legal moves for the sliding pieces, eg. Queen, Rook, Bishop
*/
void Engine::findSlidingMoves(int startSquare) {
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

//finds all the legal pawn moves
void Engine::findPawnMoves(int startSquare) {
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

//finds all the legal knight moves
void Engine::findKnightMoves(int startSquare) {
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

//finds all the legal king moves
void Engine::findKingMoves(int startSquare) {
	int color = determinePieceColor(startSquare);
	int kingSideRights = (color) ? whiteKingRights : blackKingRights;
	int queenSideRights = (color) ? whiteQueenRights : blackQueenRights;
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

//checks if a given square is legal for the king to move to
bool Engine::checkKingSquares(int inputSquare, int color) {
	
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

//Checks if  the given square is currently being targeted based on its color. Useful
bool Engine::checkTargetByColor(int inputSquare, int color) {
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
Adds the desired move to the set of all legal moves based on the input startsquare and input targetsquare for the move
*/
void Engine::addMove(int startSquare, int targetSquare) {
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

//checks to see if a move is legal when the king is in check. For example if a piece can block a check from an opponents piece
bool Engine::checkMoveHandler(int targetSquare) {
	if (checkSquares.find(targetSquare) != checkSquares.end() && checkCounter <= 1) {
		return true;
	}
	else {
		return false;
	}
}

//converts a int file into the char/string that it represents 
string Engine::fileToLetter(int file) {
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

//finds all of the targets that the opponent has. this also determines checks and pins
void Engine::findTargets() {
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
finds all of the targeted squares by the sliding pieces
*/
void Engine::slidingTargets(int startSquare) {
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

//finds the squares targeted by pawns
void Engine::pawnTargets(int startSquare) {
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

//finds the squares targeted by knights
void Engine::knightTargets(int startSquare) {
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

//finds the squares targeted by the king
void Engine::kingTargets(int startSquare) {
	for (int i = 0; i < 8; ++i) {
		int targetSquare = startSquare + directionOffsets[i];
		if (targetSquare >= 0 && targetSquare < 64) {
			addTarget(startSquare, targetSquare);
		}
	}
}

//adds a move to the set of targets based off the starting square and the targeted square
void Engine::addTarget(int startSquare, int targetSquare) {
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

//function called at the beginnning of the program to determine how many squares until the edge of the board in every direction for every square on the board
void Engine::fillNumToEdge() {
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

//determines the color of a chess piece based on that pieces current square 
int Engine::determinePieceColor(int startSquare) {
	if (squares[startSquare] == 0) {
		return -1;
	}
	else if (squares[startSquare] >> 4 == 1) {
		return 0;
	}
	else {
		return 1;
	}
}

//factory function for creating a new version of this object. Used to create new engine objects for multithreading mainly
Engine* Engine::engineCreator(string position, vector<string> startMoves,int* globalAlpha, mutex* globalAlphaMutex, int* globalBeta, mutex* globalBetaMutex) {
	handlePosition(position);
	legalMoves = startMoves;
	return  new Engine;
}
