#include "../headers/Game.h"
#include "../headers/Pawn.h"
#include "../headers/includes.h"

char Pawn::GetPiece()
{
	return 'P';
}

std::string Pawn::describePiece()
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "pawn";

	return description;
}

bool Pawn::isValidMove(Position present, Position future, Game& current_game)
{
	bool bValid = false;

	// Движение вперёд
	if (future.iColumn == present.iColumn)
	{
		// Простое движение вперед
		if ((isWhitePiece() && future.iRow == present.iRow + 1) ||
			(isBlackPiece() && future.iRow == present.iRow - 1))
		{
			if (current_game.getPieceAtPosition(future.iRow, future.iColumn) == nullptr)
			{
				bValid = true;
				return true;
			}
		}

		// Двойной ход вперёд
		else if ((isWhitePiece() && future.iRow == present.iRow + 2) ||
			(isBlackPiece() && future.iRow == present.iRow - 2))
		{
			if (firstMove == false)
			{
				bValid = true;
				firstMove = true;			//сделали первых ход
			}
			else
				throw "Can not make double move by pawn\n";
		}
		else
		{
			// Нельзя переместить
			return false;
		}
	}
	else if (1 == abs(future.iColumn - present.iColumn))
	{
		if ((isWhitePiece() && future.iRow == present.iRow + 1) ||			//здесь поменяла
			(isBlackPiece() && future.iRow == present.iRow - 1))
		{
			// Можем съесть если присутствует фигура на клетке
			if (current_game.getPieceAtPosition(future.iRow, future.iColumn))
			{
				bValid = true;
				std::cout << "Pawn captured a piece!\n";
			}
		}
		else
		{
			return false;			// Нельзя съесть
		}
	}

	// Будет ли король под шахом после хода?
	if (current_game.wouldKingBeInCheck(this, present, future, &current_game.S_enPassant) == true)
	{
		std::cout << "Move would put player's king in check\n";
		return false;
	}
	return bValid;
}

bool Pawn::isPathFree(Position startingPos, Position finishingPos, Game& current_game)
{
	return Pawn::isValidMove(startingPos, finishingPos, current_game);
}