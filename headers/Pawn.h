#pragma once
#include "Piece.h"

class Game;

/// Описание фигуры Пешка, наследует состояние класса Piece
class Pawn : public Piece
{
public:
	static const char pawnName = 'P';

	Pawn(char pieceColor) : Piece(pieceColor, pawnName) {}
	~Pawn() {}

	std::string describePiece() const override;
	bool isValidMove(Position present, Position future, Game& current_game) override;
	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:
	bool firstMove = false;		//для двойного хода
};