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

	// �������������� �������� ��� ������������ ��������
	if ((future.iRow == present.iRow) && (future.iColumn != present.iColumn) ||
		(future.iRow != present.iRow) && (future.iColumn == present.iColumn))
	{
		if (isPathFree(present, future, current_game))			// ���������, ��� �� �� ���� �����
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

bool Rook::isPathFree(Position startingPos, Position finishingPos, Game& current_game) 
{
	bool bFree = false;

	if (startingPos.iRow == finishingPos.iRow)				//�����������
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

	if (startingPos.iColumn == finishingPos.iColumn)			//�������� �� ���������
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