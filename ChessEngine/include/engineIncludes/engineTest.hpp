#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>

#include "engine.hpp"


using namespace std;


/*
This is the test bench that i built to pit two different engines against each other. Give this objest two
engines that you want to test against each other and, if desired, a starting position alongside the number
of games you want to run and it will run that many games and return how many games each engine won/lost/draw in the
terminal
*/

class EngineTest {
private:
	string startString;
	int engine1Score;
	int engine2Score;
    int totalMovesOverall;

void runGames(Engine& engine1, Engine& engine2, int numGames);

void runOneGame(Engine& engine1, Engine& engine2, int turn);

int processInfoScore(const string& engineOutput);

public:

EngineTest(Engine& engine1, Engine& engine2, int numGames, string startPosition = " rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq");




};