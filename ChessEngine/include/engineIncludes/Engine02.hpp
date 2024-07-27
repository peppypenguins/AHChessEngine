#pragma once
#include "engine.hpp"

#include<map>
#include<thread>
#include<mutex>
#include <bits/stdc++.h>

using std::map;
using std::lower_bound;


/*
This is an updated version of the Engine02 engine where it is also a multithreaded capable engine, but
i also added the alpha beta algorithm to it, alongside a quiesceSearch for more accurate evaluations. Can be used
by itself or with the EngineWrapper for multithreading
*/

class Engine02 : public Engine {
private:
	int totalMoves = 0;
	vector<string> givenMoves;
	int endBestEval = 0;
	string endBestMove = "";
	mutex* globalAlphaMutex;
	mutex* globalBetaMutex;
	int* globalAlpha;
	int* globalBeta;


	int search(int alpha, int beta,int depthLeft,int& rootDepth,int initColor);

	int quiesceSearch(int alpha, int beta, int& initColor,int& depthLeft, int& rootDepth);

	bool isCapture(string move);

	vector<string> findCaptureMoves();

	int evaluatePos();

	void undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]);

	int initValues();

	int updateMove(string& currMove, string& bestFoundMove, int& bestEval, int& currEval);

	void handleGo(const string& input) override;

	Engine* engineCreator(string position, vector<string> startMoves,int* globalAlpha, mutex* globalAlphaMutex,int* globalBeta,mutex* globalBetaMutex) override;

	void sortMoves(vector<string>& moves);

	void assignValue(vector<pair<int, string>>& givenVec, string move);

	void assignValue2(vector<pair<int, string>>& givenVec, string move);

	int pawnToPromotion(char promoteType);

	void kingMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	void pawnMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	void knightMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	void bishopMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	void rookMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	void queenMoveValue(vector<pair<int, string>>& givenVec, int startSquare, int targetSquare, int targetPieceType, string& move);

	static bool compare(const pair<int, string>& a,const pair<int, string>& b);

	void insertSortVec(vector<pair<int, string>>& givenVec, const pair<int, string>& newPair);

	int pieceTypeToValue(int pieceType) {
		switch (pieceType) {
		case 1: return INT32_MAX;
			break;
		case 2: return 100;
			break;
		case 3: return 300;
			break;
		case 4: return 325;
			break;
		case 5: return 500;
			break;
		case 6: return 900;
			break;
		default: return 0;
			break;
		}
	}

	bool globalAlphaBreak(int& initColor, int& eval, int& depthLeft, int& rootDepth);
	

public:

	Engine02(string position = "", const vector<string>& startMoves = {}, int* globalAlphaGiven = nullptr,mutex* globalAlphaMutexGiven = nullptr,
		int* globalBetaGiven = nullptr, mutex* globalBetaMutexGiven = nullptr){
		handlePosition("position" + position);
		givenMoves = startMoves;
		globalAlpha = globalAlphaGiven;
		globalAlphaMutex = globalAlphaMutexGiven;
		globalBeta = globalBetaGiven;
		globalBetaMutex = globalBetaMutexGiven;
	}

	int searchRoot(int depth, int& rootDepth, string& rootMove, vector<string>& givenMoves);

	int getBestEval() {
		return endBestEval;
	}

	string getBestMove() {
		return endBestMove;
	}

	int getTotalMoves() override {
		return totalMoves;
	}

	pair<int, string> getBestPair() override {
		pair<int, string> returnPair(endBestEval, endBestMove);
		return returnPair;
	}

	void setGivenMoves() {
		givenMoves = findLegalMoves();
	}

};