#pragma once
#include "Piece.h"

class Game;

class Bishop : public Piece
{
public:
	Bishop(char PieceColor) : Piece(PieceColor) {}
	~Bishop() {}

	char GetPiece() override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:
	
};