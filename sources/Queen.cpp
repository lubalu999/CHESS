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

	// ��������������/������������/������������ ��������
	if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn) ||
		(future.iRow != present.iRow) && (future.iColumn == present.iColumn) ||
		abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
	{
		if (isPathFree(present, future, current_game))		// ��������� ���� �� ������ �� ����
		{
			bValid = true;
		}
	}

	// ����� �� ������ ��� ����� ����� ����?
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

	// �����-������
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

	// �����-�����
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

	// ����-������
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

	// ����-�����
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

	else if (startingPos.iRow == finishingPos.iRow)				//�������� �� �����������
	{
		if (startingPos.iColumn < finishingPos.iColumn)		// �������� ������		
		{
			bFree = true;									// ��������� bFree ���������� ������

			for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
			{
				if (current_game.isSquareOccupied(startingPos.iRow, i))
				{
					bFree = false;
					std::cout << "Horizontal path to the right is not clear!\n";
				}
			}
		}

		else //if (startingPos.iColumn > finishingPos.iColumn)		// �������� �����
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

	else if (startingPos.iColumn == finishingPos.iColumn)			//�������� �� ���������
	{
		if (startingPos.iRow < finishingPos.iRow)				// �������� �����		
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

		else //if (startingPos.iColumn > finishingPos.iRow)			// �������� ����
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