#pragma once
#include "Piece.h"

class Game;

/// Описание фигуры Ладья, наследует состояние класса Piece
class Rook : public Piece
{
public:
	Rook(char PieceColor) : Piece(PieceColor) {}
	~Rook() {}

	char GetPiece() override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:
	bool firstMove = false;			//для рокировки

};