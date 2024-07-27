#include "StartScreen.hpp"


/*
constructor that initializes the start screen and allocates memory for all of its child objects
*/
StartScreen::StartScreen(sf::RenderWindow* givenWindow) : window(givenWindow){

		screenBackground = std::make_unique<sf::Texture>();
		playAI = std::make_unique<Button>(*window);
		startButton = std::make_unique<Button>(*window);
		whitePieces = std::make_unique<Button>(*window);
		blackPieces = std::make_unique<Button>(*window);


		playAI->setText("Play AI");
		playAI->setButtonPosition(sf::Vector2f((float)(window->getSize().x * 0.30), (float)(window->getSize().y * 0.4)));
		playAI->setButtonColor(sf::Color(252, 57, 3));


		startButton->setText("Start Game");
		startButton->setButtonPosition(sf::Vector2f((float)(window->getSize().x * 0.60), (float)(window->getSize().y * 0.4)));
		startButton->setButtonColor(sf::Color(3, 252, 248));
		
		whitePieces->deleteText();
		whitePieces->setButtonPosition(sf::Vector2f((float)(window->getSize().x) * 0.25, (float)(window->getSize().y) * 0.5));
		whitePieces->setButtonColor(sf::Color(240, 62, 62));
		whitePieces->setRectSize(sf::Vector2f(130.f,130.f));
		whitePieces->createSprite(std::string(REFERENCE_DIR) + "/chess_sprites.png", whitePieces->getRectSize());
		whitePieces->setSpriteRect(sf::IntRect(0, 0, 130, 130));
		whitePieces->spriteOffset(sf::Vector2f(0.f, 0.f));
		
		

		blackPieces->deleteText();
		blackPieces->setButtonPosition(sf::Vector2f((float)(window->getSize().x) * 0.35, (float)(window->getSize().y) * 0.5));
		blackPieces->setButtonColor(sf::Color(255,255,255));
		blackPieces->setRectSize(sf::Vector2f(130.f,130.f));
		blackPieces->createSprite(std::string(REFERENCE_DIR) + "/chess_sprites.png", blackPieces->getRectSize());
		blackPieces->setSpriteRect(sf::IntRect(0, 130, 130, 130));
		blackPieces->spriteOffset(sf::Vector2f(0.f, 0.f));
		

		if (!screenBackground->loadFromFile(std::string(REFERENCE_DIR) + "/chessBack2.jpg")) {
			std::cout << "Failed to load background" << std::endl;
		}
		else {
			screenBackground->loadFromFile(std::string(REFERENCE_DIR) + "/chessBack2.jpg");
			screenSprite = std::make_unique<sf::Sprite>();
			screenSprite->setTexture(*screenBackground);
		}
		
}

/*
destructor for deleting all of its child object. probably unneeded since they use smart pointers but i am now used to it
*/
StartScreen::~StartScreen(){
        if (screenBackground != nullptr)
        screenBackground.reset();
        if (screenSprite != nullptr)
        screenSprite.reset();
        if (whitePieces != nullptr)
        whitePieces.reset();
        if (blackPieces != nullptr)
        blackPieces.reset();
        if (playAI != nullptr)
        playAI.reset();
        if (startButton != nullptr)
        startButton.reset();
}

/*
draws the start screen
*/
void StartScreen::draw(){
			window->draw(*screenSprite);
			if (playAI != nullptr)
			playAI->drawButton();
			if (startButton != nullptr)
			startButton->drawButton();
			if (whitePieces != nullptr) {
				whitePieces->drawButton();
			}
			if (blackPieces != nullptr) {

				blackPieces->drawButton();
			}
}

/*
resizes the startscreen based on the current state of the window
*/
void StartScreen::resize(){
			if (window->getSize().x > window->getSize().y) {
				screenSprite->setScale(float(window->getSize().y)/1080, float(window->getSize().y)/1080);
			}
			else {
				screenSprite->setScale(float(window->getSize().x)/1080, float(window->getSize().x)/1080);
			}
}


/*
returns whether the game should be an ai game or not
*/
bool StartScreen::getGameState(){
return isAIGame;
}

/*
returns which color the player requests to play as
*/
bool StartScreen::getPlayerColor(){
return playerIsWhite;
}

/*
returns whether the game should start or not
*/
bool StartScreen::getStartGame(){
    return startGame;
}

/*
handles all of the possible outcomes based on where the mouse was clicked
*/
void StartScreen::mouseClickHandler(sf::Vector2f clickPos){
    if (whitePieces->clickedButton(clickPos)){
        playerIsWhite = true;
		whitePieces->setButtonColor(sf::Color(240, 62, 62));
		blackPieces->setButtonColor(sf::Color(255,255,255));
    } else if (blackPieces->clickedButton(clickPos)){
		playerIsWhite = false;
		blackPieces->setButtonColor(sf::Color(240, 62, 62));
		whitePieces->setButtonColor(sf::Color(0, 0, 0));
    } else if (startButton->clickedButton(clickPos)){
        startGame = true;
    } else if (playAI->clickedButton(clickPos)){
        isAIGame = !isAIGame;
        	if (isAIGame) {
				playAI->setButtonColor(sf::Color(68, 240, 62));
			}
			else {
				playAI->setButtonColor(sf::Color(252, 57, 3));
			}
    }

}
