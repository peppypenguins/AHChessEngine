# AHChessEngine
 Multi Threaded Chess Engine plus GUI

Currently being built with cmake. cmakelists file included in project to help build.

The engine is currently an alpha beta engine with some multithreading to split up the moves among several threads. Each thread determines the 
evaluation for their given set of moves and subsequently return their best move plus evaluation which is then sorted at the root to determine 
the best move.

When starting the program the user has the choice of a normal chess game where the AI is not involved or playing against the AI.
