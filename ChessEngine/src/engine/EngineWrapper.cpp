#include "EngineWrapper.hpp"
/*
Resets the engine wrapper and engines themselves to a base state and then starts a multi threaded search for the best move in the given position
*/
void EngineWrapper::handleGo(const string& input) {
	globalAlpha = INT32_MIN + 1;
	bestPair.first = INT32_MIN + 1;
	threadResult.clear();
	legalMoveStart = findLegalMoves();
	currPos = exportPosition();
	threadLaunch();
	wrapUp();
}

/*
the function where each thread does its individual work of finding its best move
*/
void EngineWrapper::threadFunc(vector<string> threadMoves) {
	Engine* workEngine = engineReference->engineCreator(currPos,threadMoves,&globalAlpha,&globalAlphaMutex,&globalBeta,&globalBetaMutex);
	workEngine->executeCmd("go");
	pair<int, string> resultPair = workEngine->getBestPair();
	int threadTotalMoves = workEngine->getTotalMoves();
	delete workEngine;
	lock_guard<mutex> lock(threadResultMutex);
	{threadResult.push_back(resultPair);
	totalMoves += threadTotalMoves;
	}
}


/*
this function works to create and then launch all of the threads to do their work, afterwords rejoins the threads and saves the best move found by each thread, before computing 
the final best move
*/
void EngineWrapper::threadLaunch() {
	const auto processor_count = std::thread::hardware_concurrency();
	int numThreads = processor_count - 1;//((processor_count / 2) + 1 < 8) ? (processor_count / 2) + 1 : 8; if you want to limit cores
	//int numThreads = 1;
	vector<thread> threadVector;
	vector<string>* threadMoves = new vector<string>[numThreads];
	int movesPerThread = (legalMoveStart.size() / numThreads);
	int extraMoves = legalMoveStart.size() % numThreads;
	int moveCounter = 0;

	for (const auto& singleMove : legalMoveStart) {
		threadMoves[moveCounter].push_back(singleMove);
		if (moveCounter == numThreads-1) {
			moveCounter = 0;
		}
		else {
			moveCounter++;
		}
	}

	for (int i = 0; i < numThreads && i < legalMoveStart.size(); ++i) {
		threadVector.emplace_back(&EngineWrapper::threadFunc, this, threadMoves[i]);
	}

	for (auto& th : threadVector) {
		th.join();
	}
	for (const auto& result : threadResult) {
		bestPair = (result.first > bestPair.first) ? result : bestPair;
	}

	delete[] threadMoves;
}

/*
wraps up the search by updating the engine to the current state and saving the best found move
*/
void EngineWrapper::wrapUp() {
	legalMoves = findLegalMoves();
	if (legalMoves.size() > 0) {
		status = -1;
		updateBoard(bestPair.second);
		findNumPieces();
		currPos = exportPosition();
	}
	else if (checkMeter == 0) {
		currPos = exportPosition();
		status = 0;
	}
	else {
		currPos = exportPosition();
		status = 1;
	}

/*	cout << "Current Evaluation: " << bestPair.first << endl;
	cout << "Best Found Move: " << bestPair.second << endl;
	cout << "Total Moves Checked: " << totalMoves << endl;
	cout << "global alpha: " << globalAlpha << endl;
	cout << "global beta: " << globalBeta << endl;
*/	
}

/*
function used for outside code to obtain the best pair found by the engine
*/
pair<int, string> EngineWrapper::getBestPair(){
	return bestPair;
}