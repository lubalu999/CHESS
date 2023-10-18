#include "../headers/Game.h"
#include "../headers/Queen.h"

char Queen::GetPiece()
{
	return 'Q';
}

std::string Queen::describePiece()
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "queen";

	return description;
}

bool Queen::isValidMove(Position present, Position future, Game& current_game) 
{
	bool bValid = false;

	// Горизонтальное/вертикальное/диагональное движение
	if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn) ||
		(future.iRow != present.iRow) && (future.iColumn == present.iColumn) ||
		abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
	{
		if (isPathFree(present, future, current_game))		// Проверяем есть ли фигуры по пути
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

bool Queen::isPathFree(Position startingPos, Position finishingPos, Game& current_game) 
{
	bool bFree = false;

	// Вверх-вправо
	if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
	{
		bFree = true;

		for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
		{
			if (current_game.isSquareOccupied(startingPos.iRow + i, startingPos.iColumn + i))
			{
				bFree = false;
				std::cout << "Diagonal path up-right is not clear!\n";
			}
		}
	}

	// Вверх-влево
	else if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
	{
		bFree = true;

		for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
		{
			if (current_game.isSquareOccupied(startingPos.iRow + i, startingPos.iColumn - i))
			{
				bFree = false;
				std::cout << "Diagonal path up-left is not clear!\n";
			}
		}
	}

	// Вниз-вправо
	else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
	{
		bFree = true;

		for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
		{
			if (current_game.isSquareOccupied(startingPos.iRow - i, startingPos.iColumn + i))
			{
				bFree = false;
				std::cout << "Diagonal path down-right is not clear!\n";
			}
		}
	}

	// Вниз-влево
	else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
	{
		bFree = true;

		for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
		{
			if (current_game.isSquareOccupied(startingPos.iRow - i, startingPos.iColumn - i))
			{
				bFree = false;
				std::cout << "Diagonal path down-left is not clear!\n";
			}
		}
	}

	else if (startingPos.iRow == finishingPos.iRow)				//Движение по горизонтали
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

	else if (startingPos.iColumn == finishingPos.iColumn)			//Движение по вертикали
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