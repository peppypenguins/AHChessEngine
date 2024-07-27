#pragma once
#include <vector>
#include <string>

#include "Piece.hpp"


using std::vector;
using std::string;



/*
This object is what is displayed when the board is waiting for the result of a promotion from the user, so until the user selects a king, queen, rook, or knight
this object is displayed
*/


class PromotionObject {

private:
sf::RenderWindow* window;
vector<Piece*> promotionPieces;
sf::Vector2f* topLeft;
float* length;
bool* isFlipped;

int whiteSquares[4] = {0,0,0,0};



void drawSquare(sf::Color color, int square);
public:
int promotionSquare = -1;

PromotionObject(int promoteSquare, sf::RenderWindow* givenWindow, sf::Vector2f* givenTopLeft, float* givenLength, bool* givenIsFlipped);
~PromotionObject();

void drawPromotion();


};