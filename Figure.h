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
		return toupper(mPieceColor) == 'W' ? true : false;
	}

	bool isBlackPiece()
	{
		return toupper(mPieceColor) == 'B' ? true : false;
	}

	

	virtual std::string describePiece() = 0;
	virtual char GetPiece() = 0;			//пересмотреть подачу аргументов (х,у)
	virtual bool isValidMove(Position present, Position future, Game& current_game) = 0;
	virtual bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) = 0;

protected:
	char mPieceColor;


};