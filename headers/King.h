#pragma once
#include "Piece.h"

class Game;

struct UnderAttack			//под атакой
{
	bool bUnderAttack = 0;
	int iNumAttackers = 0;				//количество атакующих
	Attacker attacker[9] = { 0 };		//теоретически максимальное число атакующих
};


/// Описание фигуры Король, наследует состояние класса Piece
class King : public Piece
{
public:
	King(char PieceColor) : Piece(PieceColor) {}
	~King() {}

	char GetPiece()	override;

	std::string describePiece() override;

	bool isValidMove(Position present, Position future, Game& current_game) override;

	bool isPathFree(Position startingPos, Position finishingPos, Game& current_game) override;

private:

	bool firstMove = false;			//для рокировки

};