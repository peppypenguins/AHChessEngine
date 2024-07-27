#pragma once
#include"SFML\System.hpp"
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"ReferenceConfig.h" 
#include<string.h>
#include<iostream>


/*
This file contains several classes for the overall piece class and the individual classes for each piece, each simply contains a different part of 
the chess piece texture for its individual piece. Each piece has a set value that is used throughout the project to identify each piece. If the location, size, or color of the piece 
needs to be changed call their respective functions.
*/


class Piece {
private:
protected:
sf::Texture* pieceTexture;
sf::Sprite sprite;
public:
int value = 0;

Piece();
~Piece();

void setLocation(sf::RenderWindow* window, int square, sf::Vector2f topLeft, float length, int flip = 0);
void spriteResize(sf::RenderWindow* window);
void drawPiece(sf::RenderWindow* window);
virtual void setColor(int isWhite);
};


class King : public Piece {

private:

public:
King(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(0,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 1;
}
void setColor(int isWhite) override;
};


class Pawn : public Piece {

private:

public:
Pawn(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(665,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 2;
}
void setColor(int isWhite) override;
};


class Knight : public Piece {

private:

public:
Knight(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(400,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 3;
}
void setColor(int isWhite) override;
};


class Bishop : public Piece {

private:

public:
Bishop(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(267,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 4;
}
void setColor(int isWhite) override;
};


class Rook : public Piece {

private:

public:
Rook(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(533,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 5;
}
void setColor(int isWhite) override;
};

class Queen : public Piece {

private:

public:
Queen(sf::RenderWindow* window){
sprite.setTextureRect(sf::IntRect(132,0,130,130));
spriteResize(window);
setLocation(window,0,sf::Vector2f(0.0f,0.0f),0);
setColor(true);
value = 6;
}
void setColor(int isWhite) override;
};
