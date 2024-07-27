#pragma once

#include <limits>
#include "engine.hpp"


using std::string;
using std::unordered_set;


/*
This is the initial iteration of a chess engine. Very simply does a minimax algorithm that checks every possible chess move out a certain depth 
and returns the best result
*/

class Engine00 : public Engine {
private:
	int centiPawn;
	int totalMoves;

int evaluatePos();


int search(int depth,int rootDepth, string& bestMove);


void undoMove(int currSquares[64], int currPieceCount[10], int currRights[4]);

int initValues();

int updateMove(string& currMove,string& bestFoundMove, int& bestEval, int& currEval);

void handleGo(const string& input) override;

public:

};