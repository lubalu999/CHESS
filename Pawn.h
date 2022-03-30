#pragma once
#include "Figure.h"

class Game;

/// Описание фигуры Пешка, наследует состояние класса Piece
class Pawn : public Piece
{
public:
	Pawn(char PieceColor) : Piece(PieceColor) {}
	~Pawn() {}

	char GetPiece() override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

	bool firstMove = false;		//для двойного хода

};