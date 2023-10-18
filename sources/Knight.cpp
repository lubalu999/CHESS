#include "../headers/Game.h"
#include "../headers/Knight.h"

std::string Knight::describePiece() const 
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "knight";

	return description;
}

bool Knight::isValidMove(Position present, Position future, Game& current_game)
{
	bool bValid = false;

	if (isPathFree(present, future, current_game))
	{
		bValid = true;
	}

	// Будет ли король под шахом после хода?
	if (current_game.wouldKingBeInCheck(this, present, future, &current_game.S_enPassant) == true)
	{
		std::cout << "Move would put player's king in check\n";
		return false;
	}

	return bValid;
}

bool  Knight::isPathFree(Position startingPos, Position finishingPos, Game& current_game)
{
	bool bFree = false;

	if ((2 == abs(finishingPos.iRow - startingPos.iRow)) && (1 == abs(finishingPos.iColumn - startingPos.iColumn)) ||
		(1 == abs(finishingPos.iRow - startingPos.iRow)) && (2 == abs(finishingPos.iColumn - startingPos.iColumn)))
	{
		bFree = true;
	}

	return bFree;
}