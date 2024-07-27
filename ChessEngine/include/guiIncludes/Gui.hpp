#pragma once
#include "Board.hpp"
#include "StartScreen.hpp"
#include "EngineWrapper.hpp"
#include "Engine02.hpp"


/*
The various states the gui can be in
*/
enum class state{
START,
NORMAL,
ENGINE,
PROMOTION
};




/*Class for the overall functionality of the gui, handles all of the events that occur, especially mouse clicks*/

class EngineGui {

private:
sf::RenderWindow* window;
StartScreen* initScreen;
Engine* myEngine = nullptr;
Engine* engineReference = nullptr;
Board chessBoard;
int currSquare = -1;
bool isPromotion = false;
state currState = state::START;
bool isAIGame = false;
bool playerIsWhite = true;

void handleEvents();
void update();
void render();
void keyPressEvent(sf::Keyboard::Key key);
void setBoardPos(string FEN);
void handleMouseClick(sf::Event& event);
char fileToLetter(int file);
string squaresToMove(int startSquare, int targetSquare);
void engineClickState(sf::Event& event);
void handlePromotion(sf::Event& event);
void updateState();
void startClickHandler(sf::Event& event);
void engineClickHandler(sf::Event& event);
protected:

public:

EngineGui(sf::RenderWindow* givenWindow);
~EngineGui();


void run();
};