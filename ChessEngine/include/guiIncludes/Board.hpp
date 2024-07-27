#pragma once
#include "Piece.hpp"
#include <unordered_map>
#include <unordered_set>
#include <stdio.h>
#include <string.h>
#include "Promotion.hpp"

using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::pair;
using:: std::min;


enum {
    WHITEKINGRIGHTS,
    WHITEQUEENRIGHTS,
    BLACKKINGRIGHTS,
    BLACKQUEENRIGHTS
};

enum {
EMPTY = 0,
KINGVAL = 1,
PAWNVAL = 2,
KNIGHTVAL = 3,
BISHOPVAL = 4,
ROOKVAL = 5,
QUEENVAL = 6
};


/*
Holds the board state for the gui to work with. Handles piece sprite locations and updates to the board internally through either 
being given a move to updateBoard() or through a position in handlePosition(). Several other quirks that could be cleaned some more but are useful
for uptaining data for the gui itself to use.
*/

class Board {
private:
sf::RenderWindow* window;
vector<Piece*> pieceVector;
unordered_map<int, size_t> pieceIndexMap;
sf::Vector2f topLeft;
float length = 0.0;
bool isFlipped = false;
int squares[64];
bool rights[4] = {false,false,false,false};
bool isWhiteTurn = true;
bool& isPromotion;
int pieceCounts[10] = {0,0,0,0,0,0,0,0,0,0};
int& currSquare;
bool& playerIsWhite;
//move generation variables
    vector<string> legalMoves;
	unordered_set<int> whiteTargets;
	unordered_set<int> blackTargets;
	unordered_set<int>pawnFoundTargets;
	unordered_set<int> checkSquares;
	unordered_set<int> pinnedSquares;
	unordered_set<int>wasPinnedSquares;
    unordered_map<int, unordered_set<int>> pinSquaresNew;
	unordered_set<int> legalSquares;
    int directionOffsets[16] = { 1,-1,-8,8,9,7,-7,-9, -10, -17, -15, -6, 10, 17, 15, 6 };
	int numToEdge[64][8];
    int checkMeter = 0;
	int checkCounter = 0;
    int numberWhiteMoves = 0;
	int numberBlackMoves = 0;
	int enPassantSquare = -1;
    int numPieces = 0;


void drawBoard();
void drawPieces();
Piece* createPiece(char pieceChar);
void updatePiece(int color, int square, Piece* givenPiece, char pieceChar = '\0');
void findTopLeft(float* givenLength = nullptr, sf::Vector2f* givenTopLeft = nullptr);
int findPieceBySquare(int square);
int determinePieceColor(int square);
pair<int,int> decomposeMove(string move, bool* enpassantIsTrue = nullptr, bool* castleIsTrue = nullptr, bool* promotionIsTrue = nullptr);
int charToValue(char pieceChar);
int stringToSquare(string squareString);
int pieceCountsIndex(int squareValue);
void updateRights(int pieceType, int startFile);
void castleMove(pair<int,int> movePair);
void enPassMove(pair<int,int> movePair);
void promoteMove(pair<int,int> movePair, string move);
void normalMove(pair<int,int>movePair);
void displayPromotion(int promoteSquare);
char valueToChar(int pieceVal);
char pieceToChar(int pieceType);


//move generation methods
void fillNumToEdge();
bool checkKingSquares(int inputSquare, int color);
bool checkTargetByColor(int inputSquare, int color);

    vector<string> findLegalMoves();
	void findSlidingMoves(int startSquare);
	void findPawnMoves(int startSquare);
	void findKnightMoves(int startSquare);
	void findKingMoves(int startSquare);
	void addMove(int startSquare, int targetSquare);
	string fileToLetter(int file);
	bool checkMoveHandler(int targetSquare);

    void findTargets();
	void slidingTargets(int startSquare);
	void pawnTargets(int startSquare);
	void knightTargets(int startSquare);
	void kingTargets(int startSquare);
	void addTarget(int startSquare, int targetSquare);
    void findNumPieces();


protected:


public:
PromotionObject* promotionImage = nullptr;

Board(sf::RenderWindow* givenWindow, int& givenCurrSquare, bool& isPromotionGiven, bool& givenAIColor);

void loadPosition(string FEN);
void draw();
void flipBoard();
int positionToSquare(sf::Vector2f pos);
void getBoardDim(sf::Vector2f* topLeftGiven, float* givenLength);
bool compareSquareColors(int square1, int square2);
bool isSquareEmpty(int inputSquare);
void updateBoard(string move);
void removePromotion(int selectSquare);
bool isLegalMove(string move);
void setLegalSquares(int square);
bool isCheckMate();
bool getPlayerTurn();
string exportPosition();
};