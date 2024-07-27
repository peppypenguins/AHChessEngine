#pragma once
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include"ReferenceConfig.h"

using std::unique_ptr;

/*
This is a generic button class that is used for the startscreen. can be given a sprite text, font, size, and position
to change the properties of the button. has a function used to check if the given position is within the buttons rectangle size.
*/


class Button {
private:
unique_ptr<sf::RectangleShape> rectangle;
unique_ptr<sf::Text> textBox;
unique_ptr<sf::Texture> buttonTexture;
unique_ptr<sf::Sprite> buttonSprite;
sf::Font textFont; 
sf::RenderWindow& window;
typedef void (*buttonFunction)(sf::Vector2f);
buttonFunction myButtonFunction;



public:

Button(sf::RenderWindow& givenWindow);

void setFont(sf::Font font);

void setTextSize(int size);

void setText(std::string inputString);

void setTextColor(sf::Color color);

void setButtonColor(sf::Color color);

void setButtonPosition(sf::Vector2f position);

void deleteText();

void assignButtonFunction(buttonFunction funcPtr);

void callButton(sf::Vector2f inputPosition);

void createSprite(std::string textureFile, sf::Vector2f spriteSize);

void setSpriteRect(sf::IntRect rectSize);

void setSpriteSize(sf:: Vector2f spriteSize);

void spriteOffset(sf::Vector2f offset);

sf::Vector2f getRectPos();

sf::Vector2f getRectSize();

void setRectSize(sf::Vector2f size);

bool clickedButton(sf::Vector2f clickPosition);

void drawButton();

};