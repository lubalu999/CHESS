#pragma once
#include "Piece.h"

class Game;

/// Описание фигуры Ферзь, наследует состояние класса Piece
class Queen : public Piece
{
public:
	static const char queenName = 'Q';

	Queen(char pieceColor) : Piece(pieceColor, queenName) {}
	~Queen() {}

	std::string describePiece() const override;
	bool isValidMove(Position present, Position future, Game& current_game) override;
	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

};