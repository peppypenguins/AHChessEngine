#include "Board.hpp"

/*
Constructor that sets the topleft position of the board for rendering purposes. Also calls the numToEdge function which calculates the amount of squares from the edge in every direction for every square, useful for legal move
generation.
*/
Board::Board(sf::RenderWindow* givenWindow, int& givenCurrSquare, bool& isPromotionGiven, bool& givenAIColor) : currSquare(givenCurrSquare), isPromotion(isPromotionGiven), playerIsWhite(givenAIColor){
window = givenWindow;
findTopLeft();
fillNumToEdge();
(void)findLegalMoves();
}

/*
takes a FEN string as input and loads that position into the board

*/
void Board::loadPosition(string FEN){
    pieceVector.clear();
    pieceIndexMap.clear();
    memset(&squares,0,sizeof(int)*64);

int file = 0;
int rank = 0;
for (int i = 0; i < FEN.length();++i){

    if (FEN[i] == '/'){
    file = 0;
    rank++;
    } else if (FEN[i] == ' '){
            for (i; i < FEN.length();++i){
                switch (FEN[i]){
                    case 'k': rights[BLACKKINGRIGHTS] = true;
                        break; 
                    case 'q': rights[BLACKQUEENRIGHTS] = true;
                        break;
                    case 'K': rights[WHITEKINGRIGHTS] = true;
                        break;
                    case 'Q': rights[WHITEQUEENRIGHTS] = true;
                        break;
                    case 'w': isWhiteTurn = true;
                        break;
                    case 'b': isWhiteTurn = false;
                        break;
                }
            }
    } else {
        if (std::isdigit(FEN[i])) {
            file += (FEN[i] - '0');
        } else {
            int pieceSquare = (rank * 8) + file;
            int color = (islower(FEN[i])) ? 0 : 1;
            Piece* tempPiece = createPiece(tolower(FEN[i]));
            updatePiece(color, pieceSquare, tempPiece,tolower(FEN[i]));
            file++;
            }
        }
    }   
}


/*
This function creates the specific piece based on its character representation.
*/
Piece* Board::createPiece(char pieceChar){
switch (pieceChar) {
case 'k': return new King(window);
    break;
case 'p': return new Pawn(window);
    break;
case 'n': return new Knight(window);
    break;
case 'b': return new Bishop(window);
    break;
case 'r': return new Rook(window);
    break;
case 'q': return new Queen(window);
    break;
default: return nullptr;
    break;
}
}

/*
This function will update the properties of a piece based on the the desired color, square, and its piece character
*/
void Board::updatePiece(int color, int square, Piece* givenPiece, char pieceChar){
    int colorVal = (color) ? 8 : 16;
    if (pieceIndexMap.find(square) != pieceIndexMap.end()){ // piece already in map
if (square == -1){ // set to nullptr
pieceVector[pieceIndexMap[square]] = nullptr;
pieceIndexMap[square] = -1;
} else if (givenPiece != nullptr){ // update piece
givenPiece->setColor(color);
givenPiece->setLocation(window,square,topLeft,length,isFlipped);
givenPiece->spriteResize(window);
squares[square] = charToValue(pieceChar) | colorVal;
}
    } else { //piece not in map
if (square != -1 && givenPiece != nullptr){ // create piece on valid square
givenPiece->setColor(color);
givenPiece->setLocation(window,square,topLeft,length,isFlipped);
givenPiece->spriteResize(window);
pieceVector.push_back(givenPiece);
pieceIndexMap.insert(std::make_pair(square,pieceVector.size()-1));
squares[square] = charToValue(pieceChar) | colorVal;
}
    }
}

/*
This function finds the current top left corner of the board useful for rendering objects neatly together
*/
void Board::findTopLeft(float* givenLength, sf::Vector2f* givenTopLeft){
		if (window->getSize().x > window->getSize().y) {
			topLeft.x = (window->getSize().x - window->getSize().y) / 2;
			topLeft.y = 0;
			length = window->getSize().y / 8;
		}
		else {
			topLeft.x = 0;
			topLeft.y = (window->getSize().y - window->getSize().x) / 2;
			length = window->getSize().x / 8;
		}

        if (givenLength != nullptr){
            *givenLength = length;
        }

        if (givenTopLeft != nullptr){
            *givenTopLeft = topLeft;
        }
}
/*
Function that will draw all of the squares for the chess board. Also handles coloring the current selected square and coloring the possible legal moves
*/
void Board::drawBoard(){
sf::Vector2f bottomRight(topLeft.x + length * 7, topLeft.y + length * 7);
sf::Color darkSquare(153, 107, 28);
sf::Color lightSquare(222, 193, 144);
sf::Color mixSquare(242, 181, 124);
sf::Color possibleSquare(242, 129, 124);
sf::Color darkPossibleSquare(186, 4, 28);

for (int file = 0; file < 8; ++file){
    for (int rank = 0; rank < 8; ++rank){
        int currentSquare = (rank * 8) + file;
        bool isLightSquare = (file + rank) % 2 == 0;
        sf::Vector2f position = (!isFlipped) ? sf::Vector2f(topLeft.x + (file*length), topLeft.y + (rank*length)) : sf::Vector2f(bottomRight.x - (file * length), bottomRight.y - (rank*length));
        sf::RectangleShape squareShape(sf::Vector2f(length,length));
        squareShape.setPosition(position);

        if (currSquare == currentSquare){
            squareShape.setFillColor(mixSquare);
        } else if (legalSquares.find(currentSquare) != legalSquares.end() && isLightSquare){
            squareShape.setFillColor(possibleSquare);
        } else if (legalSquares.find(currentSquare) != legalSquares.end() && !isLightSquare){
            squareShape.setFillColor(darkPossibleSquare);
        }
         else {
        squareShape.setFillColor( (isLightSquare) ? lightSquare : darkSquare);
        }
        window->draw(squareShape);
    }
}
}

/*
This function will determine all of the legal moves for a given piece, mainly used display the legal moves when a specific piece has been selected by the user
*/
void Board::setLegalSquares(int square){
    legalSquares.clear();
    if (square != -1){
    int file = square % 8;
	int rank = 8 - (square / 8) ;

    string squareString = fileToLetter(file) + std::to_string(rank);
    for (auto move : legalMoves){
        if (squareString == move.substr(0,2)){
            legalSquares.emplace(stringToSquare(move.substr(2,2)));
        }
    }
    }
}

/*
This function iterates over the piece vector and draws every piece
*/
void Board::drawPieces(){
    for (int i = 0; i < pieceVector.size(); ++i){
        if (pieceVector[i] != nullptr){
            pieceVector[i]->drawPiece(window);
        }
    }
}

/*
Draws the current board state, including the pieces, squares, and if needed the promotion object image
*/
void Board::draw(){
    drawBoard();
    drawPieces();

    if (isPromotion){
        promotionImage->drawPromotion();
    }

}
/*
Flips the board so players can alternate who is at the bottom of the screen
*/
void Board::flipBoard(){
    isFlipped = !isFlipped;

    for (int i = 0; i < 64; ++i){
        if (findPieceBySquare(i) != -1){
            pieceVector[findPieceBySquare(i)]->setLocation(window,i,topLeft,length,isFlipped);
        }
    }
}

/*
finds the iterator into the piece vector for a certain piece based on its square, returns -1 if the piece is not currently in the piece vector
*/
int Board::findPieceBySquare(int square){
    if (pieceIndexMap.find(square) != pieceIndexMap.end()){
        return pieceIndexMap.find(square)->second;
    } else {
        return -1;
    }
}

/*
returns the color of a piece based on its square, 1 = white, 0 = black, -1 = empty
*/
int Board::determinePieceColor(int square){
    if ((squares[square]&0x7) == 0){
        return -1;
    }
     else if ((squares[square] >> 4)&0x0001){
        return 0;
     } else {
        return 1;
     }


}

/*
decomposes a string move into its starting square and its target square, also determines if a move was an enpassant move, a castling move, or a promotion move
*/
pair<int,int> Board::decomposeMove(string move, bool* enpassantIsTrue, bool* castleIsTrue, bool* promotionIsTrue){
    pair<int,int> resultPair(0,0);
    int startSquare = stringToSquare(move.substr(0,2));
    int targetSquare = stringToSquare(move.substr(2,2));
    int startFile = startSquare % 8;
    int targetFile = targetSquare % 8;
    int startPieceType = (squares[startSquare]) & 0x7;
    int targetPieceType = (squares[targetSquare]) & 0x7;
    resultPair = pair(startSquare, targetSquare);

    if (enpassantIsTrue != nullptr)
    *enpassantIsTrue = (startPieceType == PAWNVAL && targetPieceType == EMPTY && startFile != targetFile) ? true : false;
    if (castleIsTrue != nullptr)
    *castleIsTrue = (startPieceType == KINGVAL && abs(startFile - targetFile) > 1) ? true : false;
    if (promotionIsTrue != nullptr)
    *promotionIsTrue = (startPieceType == PAWNVAL && (targetSquare > 55 || targetSquare < 8)) ? true : false;
    return resultPair;
}

/*
converts a character representing a piece into the pieces value for internal use
*/
int Board::charToValue(char pieceChar){
    switch(tolower(pieceChar)){
        case 'k': return KINGVAL;
            break;
        case 'p': return PAWNVAL;
            break;
        case 'n': return KNIGHTVAL;
            break;
        case 'b': return BISHOPVAL;
            break;
        case 'r': return ROOKVAL;
            break;
        case 'q': return QUEENVAL;
            break;
        default: return -1;
            break;
    }
}

/*
used to convert a 2 character string into its represented square
*/
int Board::stringToSquare(string squareString){
if (squareString.size() < 2){
    return -1;
}

	int rank = 0;
	int file = 0;
	switch (squareString[0]) {
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
    default : -1;
        break;
	}
	rank =  8 - (squareString[1] - '0');

	return (rank * 8) + file;
}

/*
updates the board based on a given string move. Plays the move given to it on the displayed board
*/
void Board::updateBoard(string move){
    if (!isPromotion){
bool isEnpass;
bool isPromote;
bool isCastle;

pair<int,int> movePair = decomposeMove(move,&isEnpass,&isCastle,&isPromote);

int pieceArrayIndex = pieceCountsIndex(movePair.first);
int targetArrayIndex = pieceCountsIndex(movePair.second);
int pieceType = squares[movePair.first] & 0x7;
int startFile = movePair.first % 8;


updateRights(pieceType,startFile);

if (isCastle){
castleMove(movePair);
} else if (isPromote){
    promoteMove(movePair,move);
} else if (isEnpass){
    enPassMove(movePair);
} else { // normal move
    normalMove(movePair);
}
isWhiteTurn = !isWhiteTurn;
    }

}

/*
the number of each piece for each color is kept track of in an array piece counts, this function will find the index into this array based on the value of a square, mainly used
in the engine portion but included here in case needed
*/
int Board::pieceCountsIndex(int squareValue){

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
updates the rights of either color based on what the piece was and the pieces initial file
*/
void Board::updateRights(int pieceType, int startFile){
    if (pieceType == KINGVAL){
        if (isWhiteTurn){
            rights[WHITEKINGRIGHTS] = false;
            rights[WHITEQUEENRIGHTS] = false;
        } else {
            rights[BLACKKINGRIGHTS] = false;
            rights[BLACKQUEENRIGHTS] = false;
        }
    } else if (pieceType == ROOKVAL){
            if (isWhiteTurn){
                if (startFile < 4){
                    rights[WHITEQUEENRIGHTS] = false;
                } else {
                    rights[WHITEKINGRIGHTS] = false;
                }
            } else {
                if (startFile < 4){
                    rights[BLACKQUEENRIGHTS] = false;
                } else {
                    rights[BLACKKINGRIGHTS] = false;
                }
            }
    }
}

/*
special function for handling castling moves and then upadting the board accordingly
*/
void Board::castleMove(pair<int,int> movePair){
int rookSquare;

if (movePair.second > movePair.first){
    rookSquare = movePair.first + 3;
    int pieceIndex = findPieceBySquare(movePair.first);
    int rookIndex = findPieceBySquare(rookSquare);
    pieceVector[pieceIndex]->setLocation(window,movePair.second,topLeft,length,isFlipped);
    pieceVector[rookIndex]->setLocation(window,movePair.first+1,topLeft,length,isFlipped);

    pieceIndexMap[movePair.first] = -1;
    pieceIndexMap[rookSquare] = -1;
    pieceIndexMap[movePair.second] = pieceIndex;
    pieceIndexMap[movePair.first + 1] = rookIndex;
    squares[movePair.second] = squares[movePair.first];
    squares[movePair.first] = 0;
    squares[movePair.first + 1] = squares[rookSquare];
    squares[rookSquare] = 0;

} else {
    rookSquare = movePair.first - 4;
    int pieceIndex = findPieceBySquare(movePair.first);
    int rookIndex = findPieceBySquare(rookSquare);
    pieceVector[findPieceBySquare(movePair.first)]->setLocation(window,movePair.second,topLeft,length,isFlipped);
    pieceVector[findPieceBySquare(rookSquare)]->setLocation(window,movePair.first - 1, topLeft,length,isFlipped);

    pieceIndexMap[movePair.first] = -1;
    pieceIndexMap[rookSquare] = -1;
    pieceIndexMap[movePair.second] = pieceIndex;
    pieceIndexMap[movePair.first - 1] = rookIndex;

    squares[movePair.second] = squares[movePair.first];
    squares[movePair.first] = 0;
    squares[movePair.first - 1] = squares[rookSquare];
    squares[rookSquare] = 0;

}

}

/*
special function for handling en passant moves and then updating the board accordingly
*/
void Board::enPassMove(pair<int,int> movePair){
    int startFile = movePair.first % 8;
    int targetFile = movePair.second % 8;
    int removeSquare = (startFile > targetFile) ? movePair.first - 1 : movePair.first + 1;
    int pieceRemoveIndex = pieceCountsIndex(removeSquare);
    pieceCounts[pieceRemoveIndex]--;
    int removeSquareIndex = findPieceBySquare(removeSquare);
    int pieceIndex = findPieceBySquare(movePair.first);
    delete pieceVector[removeSquareIndex];
    pieceVector[removeSquareIndex] = nullptr;

    pieceVector[pieceIndex]->setLocation(window,movePair.second,topLeft,length,isFlipped);

    pieceIndexMap[movePair.first] = -1;
    pieceIndexMap[movePair.second] = pieceIndex;
    pieceIndexMap[removeSquare] = -1;

    squares[movePair.second] = squares[movePair.first];
    squares[removeSquare] = 0;
    squares[movePair.first] = 0;
}

/*
called when a prmotion move is detected, handles differently if it is an engine promotion or a user promotion
*/
void Board::promoteMove(pair<int,int> movePair, string move){
    if ((squares[movePair.second]&0x7) != 0){
        int removeIndex = pieceIndexMap[movePair.second];
        int removeCountsIndex = pieceCountsIndex(movePair.second);
        pieceCounts[removeCountsIndex]--;
        pieceIndexMap[movePair.second] = -1;
        delete pieceVector[removeIndex];
        pieceVector[removeIndex] = nullptr;
    }

    if (move.back() == 'q' || move.back() == 'r' || move.back() == 'n' || move.back() == 'b'){ // engine promotion
        int pieceIndex = findPieceBySquare(movePair.first);
        pieceIndexMap[movePair.first] = -1;
        pieceIndexMap[movePair.second] = pieceIndex;
        pieceCounts[pieceCountsIndex(movePair.first)]--;
        Piece* tempPtr = createPiece(move.back());
        updatePiece(determinePieceColor(movePair.first),movePair.second,tempPtr,move.back());
        squares[movePair.first] = 0;
        pieceCounts[pieceCountsIndex(movePair.second)]++;
    } else {
        int pawnIndex = findPieceBySquare(movePair.first);
        pieceIndexMap[movePair.first] = -1;
        pieceIndexMap[movePair.second] = pawnIndex; 
        squares[movePair.second] = squares[movePair.first];
        squares[movePair.first] = 0;
         delete pieceVector[pawnIndex];
         pieceVector[pawnIndex] = nullptr;
        displayPromotion(movePair.second);
    }

}

/*
creates and starts the rendering of the promotion object until the user selects a piece type to promote to
*/
void Board::displayPromotion(int promoteSquare){
    int directionValue = (promoteSquare < 8) ? 8 : -8;
    int color = (promoteSquare < 8) ? 1 : 0;

    promotionImage = new PromotionObject(promoteSquare,window,&topLeft,&length,&isFlipped);
    isPromotion = true;

}
/*
removes the promotion object and updates the board to the selected piece based on the square that was selected
*/
void Board::removePromotion(int selectSquare){
    int adjustedRank = ((selectSquare / 8) > 3) ? abs((selectSquare/8) - 7) : selectSquare/8;
    int pawnCountRemove = pieceCountsIndex(promotionImage->promotionSquare);
    pieceCounts[pawnCountRemove]--;

    Piece* tempPtr;

    switch(adjustedRank){
        case 0: tempPtr = new Queen(window);
            break;
        case 1: tempPtr = new Rook(window);
            break;
        case 2: tempPtr = new Bishop(window);
            break;
        case 3: tempPtr = new Knight(window);
            break;
        default:
            break;
    }

    updatePiece(determinePieceColor(promotionImage->promotionSquare),promotionImage->promotionSquare,tempPtr, valueToChar(tempPtr->value));

    pieceCounts[pieceCountsIndex(promotionImage->promotionSquare)]++;

    int promoteIndex = findPieceBySquare(promotionImage->promotionSquare);
    pieceVector[promoteIndex] = tempPtr;

    delete promotionImage;
    isPromotion = false;
}

/*
converts the internal piece value representation into that pieces corresponding character
*/
char Board::valueToChar(int pieceVal){

    switch(pieceVal){
        case EMPTY: return 'z';
            break;
        case KINGVAL: return 'k';
            break;
        case PAWNVAL: return 'p';
            break;
        case KNIGHTVAL: return 'n';
            break;
        case BISHOPVAL: return 'b';
            break;
        case ROOKVAL: return 'r';
            break;
        case QUEENVAL: return 'q';
            break;
        default: return 'p';
            break;
    }
}

/*
This function handles all of the regular no special case moves and then updates the board afterword
*/
void Board::normalMove(pair<int,int>movePair){
    if ((squares[movePair.second]&0x7) != 0){//capture
        int targetIndex = findPieceBySquare(movePair.second);
        pieceCounts[pieceCountsIndex(movePair.second)]--;
        delete pieceVector[targetIndex];
        pieceVector[targetIndex] = nullptr;
    }
    if ((squares[movePair.first]&0x7) == 2 && (abs(movePair.first-movePair.second) == 16)){ // update enpassantsquare on double pawn push
        enPassantSquare = movePair.second;
    } else {
        enPassantSquare = -1;
    }

    int pieceIndex = findPieceBySquare(movePair.first);
        pieceIndexMap[movePair.second] = pieceIndex;
        pieceIndexMap[movePair.first] = -1;
        updatePiece(determinePieceColor(movePair.first),movePair.second,pieceVector[pieceIndex],valueToChar(pieceVector[pieceIndex]->value));
        squares[movePair.second] = squares[movePair.first];
        squares[movePair.first] = 0;
}

/*
converts a given pos on the screen to the corresponding square, used for mouse clicks primarily
*/
int Board::positionToSquare(sf::Vector2f pos){
    float relativeX = pos.x - topLeft.x;
    float relativeY = pos.y - topLeft.y; 

    sf::Vector2f bottomRight((length*8),(length*8));

    if (relativeX <= 0 || relativeX >= (bottomRight.x) || relativeY <= 0 || relativeY >= bottomRight.y){
        return -1;
    }
        else {
    int file = (int)(relativeX / length) % 8;
    int rank = (int)(relativeY / length) % 8;
    if (isFlipped){
        rank = 7 - rank;
        file = 7 - file;
    }
    return (rank*8) + file;
        }
}

/*
unused currently but will return the current diminsions of the board if needed
*/
void Board::getBoardDim(sf::Vector2f* topLeftGiven, float* givenLength){
    *topLeftGiven = topLeft;
    *givenLength = length;
}

/*
compares the color of two squares and if they are the same returns true, otherwise returns false
*/
bool Board::compareSquareColors(int square1, int square2){
    if (determinePieceColor(square1) == determinePieceColor(square2)){
        return true;
    } else {
        return false;
    }
}

/*
checks if a given square is empty (does not have a piece on it)
*/
bool Board::isSquareEmpty(int inputSquare){
    if ((squares[inputSquare]&0x7) == 0){
        return true;
    } else {
        return false;
    }
}

/*
searches the legal move set and checks if a certain move is enclosed inside the set
*/
bool Board::isLegalMove(string move){
    (void)findLegalMoves();
    if (!isdigit(move.back())){
        move = move.substr(0,4);
    }

    for(auto singlemove : legalMoves){
        if (!isdigit(singlemove.back())){
            singlemove = singlemove.substr(0,4);
        }
        if (move == singlemove){
            return true;
        }
    }


    return false;
}

/*
checks if the board is currently in checkmate
*/
bool Board::isCheckMate(){
    if (legalMoves.size() > 0){
        return false;
    } else {
        return true;
    }
}

/*
checks whose turn it currently is
*/
bool Board::getPlayerTurn(){
    return isWhiteTurn;
}

/*
exports the current position of the board as a FEN string
*/
string Board::exportPosition(){
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
	if (rights[WHITEKINGRIGHTS])
		returnString += 'K';
	if (rights[WHITEQUEENRIGHTS])
		returnString += 'Q';
	if (rights[BLACKKINGRIGHTS])
		returnString += 'k';
	if (rights[BLACKQUEENRIGHTS])
		returnString += 'q';
	return returnString;
}

/*
converts the internal piece value into the corresponding chara
*/
char Board::pieceToChar(int pieceType) {
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