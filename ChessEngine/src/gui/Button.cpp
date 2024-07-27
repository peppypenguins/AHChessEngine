#include "Button.hpp"

/*
cosntructor that initalizes the button and allocates memory for the rectangle shape and textbox
*/
Button::Button(sf::RenderWindow& givenWindow) : window(givenWindow){
    	rectangle = std::make_unique<sf::RectangleShape>();
		textBox = std::make_unique<sf::Text>();

		rectangle->setSize(sf::Vector2f(160.f,80.f));
		rectangle->setPosition(sf::Vector2f(960.f, 540.f));

		if (!textFont.loadFromFile(std::string(REFERENCE_DIR) + "/Merich.otf")) {

		}
		else {
			textBox->setFont(textFont);
		}

		textBox->setString("Default String");
		textBox->setCharacterSize(24);
		myButtonFunction = nullptr;
		buttonTexture = nullptr;
		buttonSprite = nullptr;
}

/*
changes the font from the default to a new font
*/
void Button::setFont(sf::Font font){
    		if (textBox != nullptr)
		textBox->setFont(font);
}
/*
changes the character size 
*/
void Button::setTextSize(int size){
		if (textBox != nullptr)
		textBox->setCharacterSize(size);
}

/*
changes the text displayed on the button
*/
void Button::setText(std::string inputString){
		if (textBox != nullptr)
			textBox->setString(inputString);
}

/*
changes the text color
*/
void Button::setTextColor(sf::Color color){
		if (textBox != nullptr)
		textBox->setFillColor(color);
}

/*
changes the button color
*/
void Button::setButtonColor(sf::Color color){
		if (rectangle != nullptr)
		rectangle->setFillColor(color);
}

/*
sets the buttons position
*/
void Button::setButtonPosition(sf::Vector2f position){
		if (rectangle != nullptr) {
			rectangle->setPosition(position);
		}
		if(textBox != nullptr)
		textBox->setPosition(sf::Vector2f(position.x + (rectangle->getSize().x / 10), position.y + (rectangle->getSize().y / 10)));
		
}
/*
deletes the text if a button without text is desired
*/
void Button::deleteText(){
		textBox.reset();
}

/*
assigns the button function, currently unused but i thought could be a cool idea to try to implement
*/
void Button::assignButtonFunction(buttonFunction funcPtr){
		myButtonFunction = funcPtr;
}

/*
was meant to be used with a user attached function, but needs some more work
*/
void Button::callButton(sf::Vector2f inputPosition){
		if (myButtonFunction != nullptr) {
			myButtonFunction(inputPosition);
		}
		else {
			std::cout << "buttonFunction is nullptr" << std::endl;
		}
}

/*
creates an image to hold on the button based on a texture file and the desired size of the sprite
*/
void Button::createSprite(std::string textureFile, sf::Vector2f spriteSize){
		buttonTexture = std::make_unique<sf::Texture>();
		if (!buttonTexture->loadFromFile(textureFile)) {
			std::cout << "Texture Failed" << std::endl;
		}
		else {
			buttonSprite = std::make_unique<sf::Sprite>();
			buttonSprite->setTexture(*buttonTexture);
			buttonSprite->setPosition(rectangle->getPosition());
		}
}

/*
sets the sprites rectangle 
*/
void Button::setSpriteRect(sf::IntRect rectSize){
	buttonSprite->setTextureRect(rectSize);
}

/*
sets the sprites size
*/
void Button::setSpriteSize(sf:: Vector2f spriteSize){
		buttonSprite->setScale(spriteSize);
}

/*
sets the position of the sprite based on a desired distance from the topleft corner of the buttons rectangle
*/
void Button::spriteOffset(sf::Vector2f offset){
		if (buttonSprite != nullptr) {
			buttonSprite->setPosition(rectangle->getPosition().x + offset.x, rectangle->getPosition().y + offset.y);
		}
}

/*
returns the current position of the button
*/
sf::Vector2f Button::getRectPos(){
		return rectangle->getPosition();
}

/*
returns the current size of the button
*/
sf::Vector2f Button::getRectSize(){
		return rectangle->getSize();
}

/*
sets the size of the buttons rectangle
*/
void Button::setRectSize(sf::Vector2f size){
		if (rectangle != nullptr) {
			rectangle->setSize(size);
		}
}

/*
checks if the button was clicked based on a given mouse position
*/
bool Button::clickedButton(sf::Vector2f clickPosition){
		if (clickPosition.x >= rectangle->getPosition().x && clickPosition.x <= rectangle->getPosition().x + rectangle->getSize().x) {
			if (clickPosition.y >= rectangle->getPosition().y && clickPosition.y <= rectangle->getPosition().y + rectangle->getSize().y) {
				return true;
			}
		}
		return false;
}

/*
draws the button and all of its images
*/
void Button::drawButton(){
		if (rectangle != nullptr)
		window.draw(*rectangle);
		if (buttonSprite != nullptr)
		window.draw(*buttonSprite);
		if (textBox != nullptr)
		window.draw(*textBox);
}