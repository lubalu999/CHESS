#pragma once
#include "Figure.h"
#include "Game.h"

/// Описание фигуры Конь, наследует состояние класса Piece
class Knight : public Piece
{
public:
	Knight(char PieceColor) : Piece(PieceColor) {}
	~Knight() {}

	char GetPiece() override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

};
	