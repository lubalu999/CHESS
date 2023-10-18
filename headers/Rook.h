#pragma once
#include "Piece.h"

class Game;

// �������� ������ �����, ��������� ��������� ������ Piece
class Rook : public Piece
{
public:
	static const char rookName = 'R';

	Rook(char pieceColor) : Piece(pieceColor, rookName) {}
	~Rook() {}

	std::string describePiece() const override;
	bool isValidMove(Position present, Position future, Game& current_game) override;
	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:
	bool firstMove = false;			//��� ���������
};