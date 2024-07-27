#pragma once
#include "engine.hpp"


#include <limits.h>

using std::string;
using std::unordered_set;
using std::thread;
using std::mutex;
using std::vector;
using std::iostream;



/*
This is the generic engine wrapper class used for multithreading different engines. This wrapper takes an engine pointer
as input and will create new versions of that engine and have one thread per engine determine the evaluation on a given set of moves and
compare the output at the end. More advanded multi threading could definately be implemented to vastly improve this, but will be saved for a 
later date.

*/

class EngineWrapper : public Engine {
private:
	vector<pair<int, string>> threadResult;
	mutex threadResultMutex;
	mutex globalAlphaMutex;
	mutex globalBetaMutex;
	int globalAlpha = INT32_MIN + 1;
	int globalBeta = INT32_MIN + 1;
	Engine* engineReference;
	vector<string> legalMoveStart;
	pair<int, string> bestPair;
	int totalMoves = 0;

	void handleGo(const string& input) override;
	void threadFunc(vector<string> threadMoves);
	void threadLaunch();
	void wrapUp();


public:
	EngineWrapper(Engine* givenEngine) {
		engineReference = givenEngine;
		bestPair.first = INT32_MIN+1;
	}

	pair<int,string> getBestPair() override;
};
