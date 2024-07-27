#pragma once

#include <limits>
#include <math.h>



#include "engine.hpp"
#include "Engine0.1.hpp"



using std::string;
using std::unordered_set;
using std::thread;
using std::mutex;
using std::vector;
using std::iostream;



/*
This is my first attempt at a wrapper class that will hold multiple engines itself and have multiple threads running
one for each engine with its own set of moves. This iteration is purpose built for the engine01 class
*/

class Engine01Wrapper : public Engine {
private:
	vector<pair<int, string>> threadResult;
	mutex threadResultMutex;

	void handleGo(const string& input) override;
	void threadFunc(vector<string> threadMoves);

public:

};