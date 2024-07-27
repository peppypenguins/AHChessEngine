#pragma once

#include "engine.hpp"


using namespace std;


/*
A chess engines that simply generates a random legal move. Mostly used for testing purposes
*/

class EngineRandom : public Engine {

void handleGo (const string& input) override;

};