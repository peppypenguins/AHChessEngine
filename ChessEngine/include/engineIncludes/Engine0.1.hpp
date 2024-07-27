#pragma once

#include <limits>
#include "engine.hpp"


using std::string;
using std::unordered_set;
using std::pair;
using std::mutex;
/*
I am going to attempt to use this object to create a multithreaded engine. My strategy is to have this object take moves as input to set it up
and then let it fly on the moves given to it. This way i can create multiple of these objects and have each thread work on one object
*/

class Engine01 : public Engine {
private:
	int totalMoves = 0;
	vector<string> givenMoves;
	int endBestEval = 0;
	string endBestMove = "";

int searchRoot(int depth, int& rootDepth, string& rootMove, vector<string>& givenMoves);

int search(int depth, int& rootDepth, string& rootMove);

int evaluatePos();

void undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]);

int initValues();

int updateMove(string& currMove, string& bestFoundMove, int& bestEval, int& currEval);

void handleGo(const string& input) override;

Engine* engineCreator(string position, vector<string> startMoves,int* globalAlpha, mutex* globalAlphaMutex,int* globalBeta, mutex* globalBetaMutex) override;

protected:

public:

Engine01(string position = "", const vector<string>& startMoves = {});

int getBestEval();

string getBestMove();

pair<int, string> getBestPair() override;

int getTotalMoves() override;


};