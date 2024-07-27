#pragma once
#include <memory>
#include "Button.hpp"

using std::unique_ptr;



/*
This is what is displayed at the start of the program before a chess game is started. Holds several button objects that the 
user can click to acheive different settings for the chess game itself. Returns these settings to the gui when the chess game begins
*/

class StartScreen{
    private:
    sf::RenderWindow* window;
unique_ptr<sf::Texture> screenBackground;
unique_ptr<sf::Sprite> screenSprite;
unique_ptr<Button> playAI;
unique_ptr<Button> whitePieces;
unique_ptr<Button> blackPieces;
unique_ptr<Button> startButton;
bool playerIsWhite = true;
bool isAIGame = false;
bool startGame = false;

public:

StartScreen(sf::RenderWindow* givenWindow);

~StartScreen();

void draw();

void resize();

bool getGameState();

bool getPlayerColor();

bool getStartGame();

void mouseClickHandler(sf::Vector2f clickPos);

};