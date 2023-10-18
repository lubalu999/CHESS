#pragma once
#include "Piece.h"

class Game;

class Bishop : public Piece
{
public:
	static const char bishopName = 'B';

	Bishop(char pieceColor) : Piece(pieceColor, bishopName) {}
	~Bishop() {}

	std::string describePiece() const override;
	bool isValidMove(Position present, Position future, Game& current_game) override;
	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:
	
};