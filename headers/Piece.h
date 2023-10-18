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
	Piece(char color, char name) : pieceColor(color), pieceName(name) {}
	~Piece() {}

	char GetColor() const
	{
		return pieceColor;
	}

	bool isWhitePiece() const 
	{
		return toupper(pieceColor) == 'W';
	}

	bool isBlackPiece() const 
	{
		return toupper(pieceColor) == 'B';
	}

	char GetPiece() const
	{
		return pieceName;
	}

	virtual std::string describePiece() const = 0;
	virtual bool isValidMove(Position present, Position future, Game& current_game) = 0;
	virtual bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) = 0;

protected:
	char pieceColor;
	char pieceName;
};