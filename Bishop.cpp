#include "Game.h"
#include "Bishop.h"

char Bishop::GetPiece()
{
	return 'B';
}

std::string Bishop::describePiece() 
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "bishop";

	return description;
}

bool Bishop::isValidMove(Position present, Position future, Game& current_game) 
{
	bool bValid = false;

	if (abs(future.iRow - present.iRow) == abs(future.iColumn - present.iColumn))
	{
		// ���������, ��� �� �� ���� �����
		if (isPathFree(present, future, current_game))
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

bool Bishop::isPathFree(Position startingPos, Position finishingPos, Game& current_game) 
{
	bool bFree = false;
	// �����-������
	if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
	{
		// ���������� = true
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
		// ���������� ���� ��������
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
		// ���������� ������
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
		// ���������� ������
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

	else
	{
		throw("Error. Diagonal move not allowed");
	}

	return bFree;
}