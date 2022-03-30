#pragma once
#include "Figure.h"

class Game;

/// �������� ������ �����, ��������� ��������� ������ Piece
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

	bool firstMove = false;		//��� �������� ����

};