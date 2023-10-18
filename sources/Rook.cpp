#include "../headers/Game.h"
#include "../headers/Rook.h"

std::string Rook::describePiece() const
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "rook";

	return description;
}

bool Rook::isValidMove(Position present, Position future, Game& current_game)
{
	bool bValid = false;

	// Горизонтальное движение или вертикальное движение
	if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn) ||
		(future.iRow != present.iRow) && (future.iColumn == present.iColumn))
	{
		if (isPathFree(present, future, current_game))			// Проверяем, нет ли по пути фигур
		{
			bValid = true;
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

bool Rook::isPathFree(Position startingPos, Position finishingPos, Game& current_game) 
{
	bool bFree = false;

	if (startingPos.iRow == finishingPos.iRow)				//горизонталь
	{
		if (startingPos.iColumn < finishingPos.iColumn)		// Движение вправо		
		{
			bFree = true;									// Установка bFree изначально истина

			for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
			{
				if (current_game.isSquareOccupied(startingPos.iRow, i))
				{
					bFree = false;
					std::cout << "Horizontal path to the right is not clear!\n";
				}
			}
		}

		else //if (startingPos.iColumn > finishingPos.iColumn)		// Движение влево
		{
			bFree = true;

			for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
			{
				if (current_game.isSquareOccupied(startingPos.iRow, i))
				{
					bFree = false;
					std::cout << "Horizontal path to the left is not clear!\n";
				}
			}
		}
	}

	if (startingPos.iColumn == finishingPos.iColumn)			//Движение по вертикали
	{
		if (startingPos.iRow < finishingPos.iRow)				// Движение вверх		
		{
			bFree = true;

			for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
			{
				if (current_game.isSquareOccupied(i, startingPos.iColumn))
				{
					bFree = false;
					std::cout << "Vertical path up is not clear!\n";

					return bFree;
				}
			}
		}

		else //if (startingPos.iColumn > finishingPos.iRow)			// Движение вниз
		{
			bFree = true;

			for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
			{
				if (current_game.isSquareOccupied(i, startingPos.iColumn))
				{
					bFree = false;
					std::cout << "Vertical path down is not clear!\n";
				}
			}
		}
	}

	return bFree;
}