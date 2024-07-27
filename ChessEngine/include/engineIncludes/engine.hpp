#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include<set>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <random>
#include<thread>
#include<mutex>

using namespace std;


/*
This is the main engine class that all the other engines derive from. This class keeps track of the current state of the board and can have the board updated either
via a direct move or by being given a FEN position. Class will handle legal move generation via the legalmove function, the most important part of the base engine class.
The engine currently only truly handles the "go" and "position" commands by giving the executecmd function those strings. This can be expanded by adding new function
pointers to the ucicmds array of function pointers. Probably unnecassary to use function pointers there but i was attempting to learn more about them.
*/

class Engine {
protected :
	typedef void(Engine::* uciFunc)(const std::string& input);
	vector<string> legalMoves;
	int squares[64];
	unordered_map<string, int> acceptedCommands;
	unordered_set<int> whiteTargets;
	unordered_set<int> blackTargets;
	unordered_set<int>pawnFoundTargets;
	unordered_set<int> checkSquares;
	unordered_set<int> pinnedSquares;
	unordered_set<int>wasPinnedSquares;
	string currPos;
	unordered_map<int, unordered_set<int>> pinSquaresNew;

	int pieceMove;
	int numPieces;
	int directionOffsets[16] = { 1,-1,-8,8,9,7,-7,-9, -10, -17, -15, -6, 10, 17, 15, 6 };
	int numToEdge[64][8];
	int checkMeter;
	int checkCounter;
	int isWhiteTurn;
	int whiteKingRights;
	int whiteQueenRights;
	int blackKingRights;
	int blackQueenRights;
	int numberWhiteMoves;
	int numberBlackMoves;
	int enPassantSquare;
	int status = -1;

	int pieceCounts[10];




	void fillNumToEdge();
	int determinePieceColor(int startSquare);
	bool checkKingSquares(int inputSquare, int color);
	bool checkTargetByColor(int inputSquare, int color);
	int stringToSquare(string inputString);

	vector<string> findLegalMoves();
	void findSlidingMoves(int startSquare);
	void findPawnMoves(int startSquare);
	void findKnightMoves(int startSquare);
	void findKingMoves(int startSquare);
	void addMove(int startSquare, int targetSquare);
	string fileToLetter(int file);
	bool checkMoveHandler(int targetSquare);


	void processArrayPart(int start, int end);
	void findTargets();
	void slidingTargets(int startSquare);
	void pawnTargets(int startSquare);
	void knightTargets(int startSquare);
	void kingTargets(int startSquare);
	void addTarget(int startSquare, int targetSquare);

	int pieceCountsIndex(int squareValue);
	void findNumPieces();
	string exportPosition();
	char pieceToChar(int pieceType);
	int charToPiece(char inputChar);
	void updateBoard(string move);
	void handleUCI(const string& input);
	void handleIsReady(const string& input);
	void handlePosition(const string& FEN);
   virtual void handleGo(const string& input);

	uciFunc uciCmds[4];



public : 
	string bestMove;

	Engine() {
		reset();

	}


	void reset() {
		fillNumToEdge();
		string currPos = "";
		pieceMove = 0;
		numPieces = 0;
		checkCounter = 0;
		checkMeter = 0;
		isWhiteTurn = 1;
		status = -1;
		whiteKingRights = 1;
		whiteQueenRights = 1;
		blackKingRights = 1;
		blackQueenRights = 1;
		numberWhiteMoves = -1;
		numberBlackMoves = -1;
		enPassantSquare = -1;


		uciCmds[0] = &Engine::handleUCI;
		uciCmds[1] = &Engine::handleIsReady;
		uciCmds[2] = &Engine::handlePosition;
		uciCmds[3] = &Engine::handleGo;

		acceptedCommands["uci"] = 0;
		acceptedCommands["isready"] = 1;
		acceptedCommands["position"] = 2;
		acceptedCommands["go"] = 3;
	}

	virtual ~Engine() = default;


	virtual Engine* engineCreator(string position, std::vector<string>,int* globalAlpha,mutex* globalAlphaMutex, int* globalBeta, mutex* globalBetaMutex);



	virtual pair<int, string> getBestPair() {
		pair<int, string> tempPair(0, currPos);
		return tempPair;
	}


	void executeCmd(const string& input);

	void outputInfo() {
		//cout << legalMoves.size() << endl << bestMovePair.first << endl << bestMovePair.second << endl;
		for (int i = 0; i < 64; ++i) {
			cout << squares[i] << endl;
		}

	}



	int getStatus() {
		return status;
	}

	string getPos() {
		return currPos;
	}

	int getPieceMove() {
		return pieceMove;
	}

	int getNumPieces() {
		return numPieces;
	}

	virtual int getTotalMoves() {
		return 0;
	}
};


