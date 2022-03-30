#include "Game.h"
#include "King.h"

char King::GetPiece()		
{
	return 'K';
}

std::string King::describePiece() 
{
	std::string description;

	if (isWhitePiece())
		description += "White ";
	else
		description += "Black ";

	description += "king";

	return description;
}




//bool King::isKingInCheck(char iColor, IntendedMove* pintended_move, Game& current_game)			//iColor - ���� ������
//{
//	bool bCheck = false;
//
//	Position king = { 0 };
//
//	// ���������� ���������, �������� �� �������������� ��� ����� �������
//	if (nullptr != pintended_move && 'K' == pintended_move->chPiece->GetPiece())
//	{
//		king.iRow = pintended_move->to.iRow;
//		king.iColumn = pintended_move->to.iColumn;
//	}
//	else
//	{
//		king = current_game.findKing(iColor);
//	}
//
//	UnderAttack king_attacked = isUnderAttack(king.iRow, king.iColumn, iColor, pintended_move);
//
//	if (true == king_attacked.bUnderAttack)
//	{
//		bCheck = true;
//	}
//
//	return bCheck;
//}
//
//UnderAttack King::isUnderAttack(int iRow, int iColumn, int iColor, IntendedMove* pintended_move)
//{
//	return { 0 };
//}
//
//bool King::playerKingInCheck(IntendedMove* intended_move, Game& current_game) const
//{
//	return isKingInCheck(current_game.getCurrentTurn(), intended_move, current_game);
//}

bool King::isValidMove(Position present, Position future, Game& current_game) 
{
	bool bValid = false;
	
	// ��� �� 1 ������
	if ((future.iRow - present.iRow >= -1 && future.iRow - present.iRow <= 1) &&
		(future.iColumn - present.iColumn >= -1) && (future.iColumn - present.iColumn <= 1))
	{
		bValid = true;
	}
	// ���������
	else if (future.iRow == present.iRow && abs(future.iColumn - present.iColumn) == 2)
	{
		// ��������� ��������� ������ � ��������� �������:
		// 1. ������ �� ��� �����
		if (current_game.playerKingInCheck())
		{
			return false;
		}

		// 2. ��� ����� ����� ������ � ������
		if (!isPathFree(present, future, current_game))
		{
			return false;
		}
	}

	// 3. ������ � ����� �� ������ ���� ������ ����;
	// 4. ������ �� ������ ��������� ����� ����, ������� ������� ������ ����������.
	if (future.iColumn > present.iColumn)
	{
		// ���� ������� future.iColumn ������ present.iColumn, ������ ��� ����������� �����
		if (firstMove == true)			//�������� ����� ����?
		{
			return false;
		}
		else
		{
			// ���������, �� ��������� �� ������, ������� ���������� ������
			UnderAttack square_skipped = current_game.isUnderAttack(present.iRow, present.iColumn + 1, current_game.getCurrentTurn());

			if (square_skipped.bUnderAttack == false)
			{
				// �������� S_castling ���������
				current_game.S_castling.bApplied = true;

				// ������� ��������� �����
				current_game.S_castling.rook_before.iRow = present.iRow;
				current_game.S_castling.rook_before.iColumn = present.iColumn + 3;

				// ������� ��������� �����
				current_game.S_castling.rook_after.iRow = future.iRow;
				current_game.S_castling.rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

				bValid = true;
			}
		}
	}

	else //if (future.iColumn < present.iColumn)
	{
		// ���� present.iColumn ������ ��� future.iColumn, ������ ��� ��������� �����
		if (firstMove == true)			//�������� ����� ����?
		{
			return false;
		}
		else
		{
			// ���������, �� ��������� �� ������, ������� ���������� ������
			UnderAttack square_skipped = current_game.isUnderAttack(present.iRow, present.iColumn - 1, current_game.getCurrentTurn());
			
			if (false == square_skipped.bUnderAttack)
			{
				// ��������� S_castling ���������
				current_game.S_castling.bApplied = true;

				// ������� ��������� �����
				current_game.S_castling.rook_before.iRow = present.iRow;
				current_game.S_castling.rook_before.iColumn = present.iColumn - 4;

				// ������� ��������� �����
				current_game.S_castling.rook_after.iRow = future.iRow;
				current_game.S_castling.rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

				bValid = true;
			}
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

bool King::isPathFree(Position startingPos, Position finishingPos, Game& current_game)
{
	bool bFree = false;

	//��� ���������
	if (abs(startingPos.iColumn - finishingPos.iColumn) == 2)
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

	//�������� �� 1 ������
	if (!current_game.isSquareOccupied(startingPos.iRow, startingPos.iColumn))
	{
		bFree = true;
	}

	return bFree;
}