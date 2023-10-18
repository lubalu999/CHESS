#pragma once
#include <iostream>
#include "structs.h"

class Game;

/*!
	Abstract class piece
*/
class Piece
{
public:
	Piece(char PieceColor) : mPieceColor(PieceColor) {}
	~Piece() {}

	char GetColor() {
		return mPieceColor;
	}

	bool isWhitePiece()
	{
		return toupper(mPieceColor) == 'W';
	}

	bool isBlackPiece()
	{
		return toupper(mPieceColor) == 'B';
	}
	
	virtual std::string describePiece() = 0;
	virtual char GetPiece() = 0;
	virtual bool isValidMove(Position present, Position future, Game& current_game) = 0;
	virtual bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) = 0;

protected:
	char mPieceColor;
};