#pragma once
#include "Piece.h"

class Game;

/// Описание фигуры Ферзь, наследует состояние класса Piece
class Queen : public Piece
{
public:
	Queen(char PieceColor) : Piece(PieceColor) {}
	~Queen() {}

	char GetPiece() override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

};