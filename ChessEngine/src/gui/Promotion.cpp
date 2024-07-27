#include "Promotion.hpp"


/*
constructor that primarily creates 4 new piece objects one for each possible promotion and sets their location based on the position of where the pawn is promoting
*/
PromotionObject::PromotionObject(int promoteSquare, sf::RenderWindow* givenWindow, sf::Vector2f* givenTopLeft,float* givenLength, bool* givenIsFlipped){

    window = givenWindow;
    topLeft = givenTopLeft;
    length = givenLength;
    isFlipped = givenIsFlipped;

    int directionValue = (promoteSquare < 8) ? 8 : -8;
    int color = (promoteSquare < 8) ? 1 : 0;

    promotionSquare = promoteSquare;

    Piece* tempQueen = new Queen(window);
    Piece* tempRook = new Rook(window);
    Piece* tempKnight = new Knight(window);
    Piece* tempBishop = new Bishop(window);

    tempQueen->setLocation(window, promoteSquare, *topLeft, *length,*isFlipped);
	tempRook->setLocation(window, promoteSquare + directionValue, *topLeft, *length,*isFlipped);
	tempBishop->setLocation(window, promoteSquare + directionValue*2, *topLeft, *length,*isFlipped);
	tempKnight->setLocation(window, promoteSquare + directionValue*3, *topLeft, *length,*isFlipped);
	tempQueen->setColor(color);
	tempRook->setColor(color);
	tempBishop->setColor(color);
	tempKnight->setColor(color);
	promotionPieces.push_back(tempQueen);
	promotionPieces.push_back(tempRook);
	promotionPieces.push_back(tempBishop);
	promotionPieces.push_back(tempKnight);

    whiteSquares[0] = promoteSquare;
    whiteSquares[1] = promoteSquare + directionValue;
    whiteSquares[2] = promoteSquare + (2*directionValue);
    whiteSquares[3] = promoteSquare + (3*directionValue);

}

/*
destructor used for deleting all of the allocated pieces when the promotion object was created
*/
PromotionObject::~PromotionObject(){
    for (int i = 0; i < promotionPieces.size(); ++i){
        if (promotionPieces[i] != nullptr){
            delete promotionPieces[i];
        }
    }
}

/*
draws the white squares underneath the pieces 
*/
void PromotionObject::drawSquare(sf::Color color, int square){
		int rank = square / 8;
		int file = square % 8;
		if ((*isFlipped)) {
			rank = 7 - rank;
			file = 7 - file;
		}
		sf::Vector2f position(topLeft->x + (file * *length), topLeft->y + (rank * *length));
		sf::RectangleShape squareShape(sf::Vector2f(*length, *length));
		squareShape.setPosition(position);
		squareShape.setFillColor(color);
		window->draw(squareShape);


}

/*
draws the actual promotion object
*/
void PromotionObject::drawPromotion(){
    		sf::Color whiteSquare(255, 255, 255);
		for (int i = 0; i < promotionPieces.size(); i++) {
			if (promotionPieces[i] != nullptr) {
				drawSquare(whiteSquare, whiteSquares[i]);
				promotionPieces[i]->drawPiece(window);
			}
		}
}