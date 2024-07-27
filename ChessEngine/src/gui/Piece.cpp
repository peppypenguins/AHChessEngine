#include "Piece.hpp"


/*
constructor for the overarching piece class. all pieces share the same texture just use different parts so that is assigned here
*/
Piece::Piece(){
	pieceTexture = new sf::Texture;

	if (!pieceTexture->loadFromFile(std::string(REFERENCE_DIR) + "/chess_sprites.png")){
		std::cerr << "Failed to load image: " << std::string(REFERENCE_DIR) + "/chess_sprites.png" << std::endl;
	} else {
			sprite.setTexture(*pieceTexture);
	}
}


Piece::~Piece(){
delete pieceTexture;
}



/*
sets the location of a piece based on the boards current dimensions, the square the piece should be on, and whether the board is currently flipped
*/
void Piece::setLocation(sf::RenderWindow* window, int square, sf::Vector2f topLeft, float length, int flip){
	int rank = square / 8;
	int file = square % 8;
	if (flip == 0) {
		sf::Vector2f position(topLeft.x + (length * file), topLeft.y + (length * rank));
		sprite.setPosition(position);
	}
	else {
		rank = 7 - rank;
		file = 7 - file;
		sf::Vector2f position(topLeft.x + (length * file), topLeft.y + (length * rank));
		sprite.setPosition(position);
	}
}


/*
resizes the sprite based on the size of hte current window
*/
void Piece::spriteResize(sf::RenderWindow* window){
	if (window->getSize().x > window->getSize().y) {
		sprite.setScale(float(window->getSize().y) / 1080, float(window->getSize().y) / 1080);
	}
	else {
		sprite.setScale(float(window->getSize().x) / 1080, float(window->getSize().x) / 1080);		
	}
}


/*
sets the color of the piece by changing the part of the texture that the piece should use. virtual function so the actual piece function should always be called
*/
void Piece::setColor(int isWhite){
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(0, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(0, 130, 130, 130));
	}
}

/*
draws the piece
*/
void Piece::drawPiece(sf::RenderWindow* window){
	window->draw(sprite);
}


/*
sets the color of the king by changing the part of the texture that the piece should use
*/
void King::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(0, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(0, 130, 130, 130));
	}

}


/*
sets the color of the pawn by changing the part of the texture that the piece should use
*/
void Pawn::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(665, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(665, 130, 130, 130));
	}

}


/*
sets the color of the knight by changing the part of the texture that the piece should use
*/
void Knight::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(400, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(400, 130, 130, 130));
	}

}


/*
sets the color of the bichop by changing the part of the texture that the piece should use
*/
void Bishop::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(267, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(267, 130, 130, 130));
	}

}


/*
sets the color of the rook by changing the part of the texture that the piece should use
*/
void Rook::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(533, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(533, 130, 130, 130));
	}
}

/*
sets the color of the queen by changing the part of the texture that the piece should use
*/
void Queen::setColor(int isWhite) {
	if (isWhite) {
		sprite.setTextureRect(sf::IntRect(132, 0, 130, 130));
	}
	else {
		sprite.setTextureRect(sf::IntRect(132, 130, 130, 130));
	}
}



