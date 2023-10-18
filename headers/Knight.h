#pragma once
#include "Piece.h"
#include "Game.h"

/// Описание фигуры Конь, наследует состояние класса Piece
class Knight : public Piece
{
public:
	static const char knightName = 'N';

	Knight(char pieceColor) : Piece(pieceColor, knightName) {}
	~Knight() {}

	std::string describePiece() const override;
	bool isValidMove(Position present, Position future, Game& current_game) override;
	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

};
	