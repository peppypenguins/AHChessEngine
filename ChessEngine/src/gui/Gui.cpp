#include "Gui.hpp"
/*Constructor that inits the gui by creating the start screen and setting the starting pos for the actual chessboard. Also sets the window framerate*/
EngineGui::EngineGui(sf::RenderWindow* givenWindow) : chessBoard(givenWindow,currSquare, isPromotion, playerIsWhite){
window = givenWindow;
window->setFramerateLimit(60);
initScreen = new StartScreen(window);
setBoardPos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq");
}

/*destructor that simply deletes the GUIs objects if they have allocated memory*/
EngineGui::~EngineGui(){
    if (initScreen != nullptr){
        delete initScreen;
    }
    if (myEngine != nullptr){
        delete myEngine;
    }
    if (engineReference != nullptr){
        delete engineReference;
    }
}

/*This starts the gui program, goes through a basic loop of handling events, eg mouseclicks, then updates based off of what state the board is in and 
what occured during the handle events portion. Finally renders the complete window*/
void EngineGui::run(){
    while(window->isOpen()){
handleEvents();
updateState();
render();
    }
}

/*Handles all events, right now the only events are the window closing, a keypress which can only be esc to end program or 'f' to flip the board, and mouseclicks for the overall functionality*/
void EngineGui::handleEvents(){
sf::Event event;

while(window->pollEvent(event)){
    if (event.type == sf::Event::Closed)
        window->close();

    else if (event.type == sf::Event::KeyPressed){
        keyPressEvent(event.key.code);
    }
    else if (event.type == sf::Event::MouseButtonPressed){
        handleMouseClick(event);
    }
}


}

/*deprecated because i forgot i made this function, replaced with updateState*/
void EngineGui::update(){

}
/*renders the screen, will either render the startscreen or the board state depending on the guis current state*/
void EngineGui::render(){
    window->clear();
    if (currState == state::START){
    initScreen->draw();
    } else {
    chessBoard.draw();
    }
    window->display();
}
/*handles the different key presses, right now only supports the keys 'esc' for ending the program, and 'F' for flipping the board*/
void EngineGui::keyPressEvent(sf::Keyboard::Key key){
    switch(key){
        case(sf::Keyboard::Escape) : window->close();
            break;
        case(sf::Keyboard::F) :
        if (!isPromotion)
         chessBoard.flipBoard();
            break;
        default:
            break;
    }

}
/*this function simply sets the board position for the chessboard, takes a FEN string as input for the pos*/
void EngineGui::setBoardPos(string FEN){
    chessBoard.loadPosition(FEN);
}
/*handles the different mouse clicks, just calls each states mouse button function*/
void EngineGui::handleMouseClick(sf::Event& event){
    switch (currState){
        case state::NORMAL: engineClickState(event);
            break;
        case state::START: startClickHandler(event);
            break;
        case state::ENGINE: engineClickHandler(event);
            break;
        case state::PROMOTION: handlePromotion(event);
            break;
        default:
            break;
    }
}
//simple function used to convert the int representation of the rank on a board to its character for other functions to use
char EngineGui::fileToLetter(int file){
    	switch (file) {
	case 0:return 'a';
		break;
	case 1:return 'b';
		break;
	case 2: return 'c';
		break;
	case 3: return 'd';
		break;
	case 4: return 'e';
		break;
	case 5: return 'f';
		break;
	case 6: return 'g';
		break;
	case 7: return 'h';
		break;
	default: return 'z';
		break;
	}
}

/* This function converts tow input squares (the start square and the target square) into a move in a string format to be sent to chess engines and guis to use and process*/
string EngineGui::squaresToMove(int startSquare, int targetSquare){
    	int startFile = startSquare % 8;
	int startRank = 8 - (startSquare / 8) ;
	int targetFile = targetSquare % 8;
	int targetRank = 8 - (targetSquare / 8);
	std::string move = fileToLetter(startFile) + std::to_string(startRank) + fileToLetter(targetFile) + std::to_string(targetRank);
    return move;
}

/*This is the function that handles mouse clicks for a normal chess game without an ai. Simply determines whether there was an attempt to make 
a chess move, if there was and the move was legal update the board, otherwise update what the current square is*/
void EngineGui::engineClickState(sf::Event& event){
int clickSquare = chessBoard.positionToSquare(sf::Vector2f((float)(event.mouseButton.x), (float)(event.mouseButton.y)));
if (clickSquare == -1){
    currSquare = -1;
    chessBoard.setLegalSquares(-1);
} else {
    string move = squaresToMove(currSquare,clickSquare);
    bool isLegalMove = chessBoard.isLegalMove(move);
    if (currSquare != -1 && chessBoard.compareSquareColors(currSquare,clickSquare)){
        currSquare = clickSquare;
        chessBoard.setLegalSquares(currSquare);
    } else if (currSquare != -1 && currSquare != clickSquare && !chessBoard.isSquareEmpty(currSquare) && isLegalMove){
         chessBoard.updateBoard(move);
        currSquare = -1;
        chessBoard.setLegalSquares(-1);
    } else if (currSquare != clickSquare){
        currSquare = clickSquare;
        chessBoard.setLegalSquares(currSquare);
    }
}
}

/* handles mouse clicks when the gui is in the promotion state. When the mouse is clicked determine what square the mouse was in and determine if a 
promotion piece was chosen. If so remove the promotion images from the board and return to a normal engine state*/
void EngineGui::handlePromotion(sf::Event& event){
int promotionSquare = chessBoard.promotionImage->promotionSquare;
int clickSquare = chessBoard.positionToSquare(sf::Vector2f((float)(event.mouseButton.x), (float)(event.mouseButton.y)));
int possSquares[4];
int flag = false;
if (promotionSquare < 10){
    possSquares[0] = promotionSquare;
    possSquares[1] = promotionSquare + 8;
    possSquares[2] = promotionSquare + 16;
    possSquares[3] = promotionSquare + 24;
} else {
    possSquares[0] = promotionSquare;
    possSquares[1] = promotionSquare - 8;
    possSquares[2] = promotionSquare - 16;
    possSquares[3] = promotionSquare - 24;
}

for (int i = 0; i < 4; ++i){
    if (clickSquare == possSquares[i]){
        flag = true;
    }
}

if (flag)
chessBoard.removePromotion(clickSquare);
}

/*This function handles all of the logic of what should happen in each state that the gui could be in at any point. This function should be cleaned up some*/
void EngineGui::updateState(){
    if (currState == state::START){
        if(initScreen != nullptr){
            if(initScreen->getStartGame()){
                isAIGame = initScreen->getGameState();
                playerIsWhite = initScreen->getPlayerColor();
                if (isAIGame){
                    engineReference = new Engine02();
                    myEngine = new EngineWrapper(engineReference);
                    currState = state::ENGINE;                   
                } else {
                    currState = state::NORMAL;
                }
                delete initScreen;
                initScreen = nullptr;
            }
        }
    }
     else if (currState == state::PROMOTION){
        if (!isPromotion){
            currState = (isAIGame) ? state::ENGINE : state::NORMAL;
        }
    }
    else if (isPromotion && currState != state::START){
        currState = state::PROMOTION;
    } else if (currState == state::NORMAL){
        if (chessBoard.isCheckMate()){
            initScreen = new StartScreen(window);
            currState = state::START;
            setBoardPos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq");
        }
    } else if (currState == state::ENGINE){
         if (chessBoard.isCheckMate()){
            delete myEngine;
            myEngine = nullptr;
            delete engineReference;
            engineReference = nullptr;
            initScreen = new StartScreen(window);
            currState = state::START;
            setBoardPos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR KQkq");
         } else if (playerIsWhite != chessBoard.getPlayerTurn()){
           myEngine->executeCmd("position" + chessBoard.exportPosition());
           myEngine->executeCmd("go");
           chessBoard.updateBoard(myEngine->getBestPair().second);
         }
    }
}

/*handles mouse clicks when in the start screen state. simply calls the start screens function for dealing with mouse inputs*/
void EngineGui::startClickHandler(sf::Event& event){
    if (initScreen != nullptr){
        initScreen->mouseClickHandler(sf::Vector2f((float)(event.mouseButton.x), (float)(event.mouseButton.y)));
    }
}

/*
handles mouse clicks when the gui is in the engine state. Checks to see if it is the players turn, if so call the normal state mouse handler and treat
it as if there was no ai for this move
*/
void EngineGui::engineClickHandler(sf::Event& event){
    if (playerIsWhite == chessBoard.getPlayerTurn()){
        engineClickState(event);
    }
}