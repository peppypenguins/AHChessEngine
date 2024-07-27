#include "Engine01Wrapper.hpp"


/*
function creates multiple threads, one for each engine object created and sends them out to work on some set of moves. checks the final results from each at the end and returns the best move out of them
*/
void Engine01Wrapper::handleGo(const string& input) {
	vector<string> legalMoveStart = findLegalMoves();
	currPos = exportPosition();
	const auto processor_count = std::thread::hardware_concurrency();
	int numThreads = processor_count + 1;
	vector<thread> threadVector;
	vector<string>* threadMoves = new vector<string>[numThreads];
	int movesPerThread = (legalMoveStart.size() / numThreads);
	int extraMoves = legalMoveStart.size() % numThreads;
	int moveCounter = 0;
	pair<int, string> bestPair(INT32_MIN, "");
	for (auto singleMove : legalMoveStart) {
		int threadNumber = moveCounter / (movesPerThread + (moveCounter > extraMoves ? 1 : 0));
		threadMoves[threadNumber].push_back(singleMove);
		moveCounter++;
	}

	for (int i = 0; i < numThreads; ++i) {
		threadVector.emplace_back(&Engine01Wrapper::threadFunc,this, threadMoves[i]);
	}

	for (auto& th : threadVector) {
		th.join();
	}

	for (int i = 0; i < threadResult.size(); ++i) {
		bestPair = (threadResult[i].first > bestPair.first) ? threadResult[i] : bestPair;
	}
	
	currPos = exportPosition();
	if (legalMoveStart.size() > 0) {
		status = -1;
		bestMove = bestPair.first;
	}
	else if (checkMeter == 0) {
		status = 0;
	}
	else {
		status = 1;
	}



	cout << "Current Evaluation : " << bestPair.first << endl;
	cout << "Best Found Move : " << bestPair.second << endl;
	delete[] threadMoves;
}

/*
The work that is actually done by each thread when searching for their best move
*/
void Engine01Wrapper::threadFunc(vector<string> threadMoves) {
	Engine01 engineTemp(currPos, threadMoves);
	engineTemp.executeCmd("go");
	pair<int, string> resultPair = engineTemp.getBestPair();
	lock_guard<mutex> lock(threadResultMutex);
	threadResult.push_back(resultPair);
}