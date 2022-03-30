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




//bool King::isKingInCheck(char iColor, IntendedMove* pintended_move, Game& current_game)			//iColor - цвет игрока
//{
//	bool bCheck = false;
//
//	Position king = { 0 };
//
//	// Необходимо проверить, является ли предполагаемый ход самим королем
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
	
	// Ход на 1 клетку
	if ((future.iRow - present.iRow >= -1 && future.iRow - present.iRow <= 1) &&
		(future.iColumn - present.iColumn >= -1) && (future.iColumn - present.iColumn <= 1))
	{
		bValid = true;
	}
	// рокировка
	else if (future.iRow == present.iRow && abs(future.iColumn - present.iColumn) == 2)
	{
		// Рокировка разрешена только в следующих случаях:
		// 1. Король не под шахом
		if (current_game.playerKingInCheck())
		{
			return false;
		}

		// 2. Нет фигур между королём и ладьей
		if (!isPathFree(present, future, current_game))
		{
			return false;
		}
	}

	// 3. Король и ладья не должны были делать ходы;
	// 4. Король не должен проходить через поле, которое атакует фигура противника.
	if (future.iColumn > present.iColumn)
	{
		// Если столбец future.iColumn больше present.iColumn, значит это королевский фланг
		if (firstMove == true)			//дописать ладью сюда?
		{
			return false;
		}
		else
		{
			// Проверяем, не атакована ли клетка, которую пропускает король
			UnderAttack square_skipped = current_game.isUnderAttack(present.iRow, present.iColumn + 1, current_game.getCurrentTurn());

			if (square_skipped.bUnderAttack == false)
			{
				// Заполним S_castling структуру
				current_game.S_castling.bApplied = true;

				// Текущее положение ладьи
				current_game.S_castling.rook_before.iRow = present.iRow;
				current_game.S_castling.rook_before.iColumn = present.iColumn + 3;

				// Будущее положение ладьи
				current_game.S_castling.rook_after.iRow = future.iRow;
				current_game.S_castling.rook_after.iColumn = present.iColumn + 1; // future.iColumn -1

				bValid = true;
			}
		}
	}

	else //if (future.iColumn < present.iColumn)
	{
		// Если present.iColumn больше чем future.iColumn, значит это ферзевыйй фланг
		if (firstMove == true)			//дописать ладью сюда?
		{
			return false;
		}
		else
		{
			// Проверяем, не атакована ли клетка, которую пропускает король
			UnderAttack square_skipped = current_game.isUnderAttack(present.iRow, present.iColumn - 1, current_game.getCurrentTurn());
			
			if (false == square_skipped.bUnderAttack)
			{
				// Заполняем S_castling структуру
				current_game.S_castling.bApplied = true;

				// Текущее положение ладьи
				current_game.S_castling.rook_before.iRow = present.iRow;
				current_game.S_castling.rook_before.iColumn = present.iColumn - 4;

				// Будущее положение ладьи
				current_game.S_castling.rook_after.iRow = future.iRow;
				current_game.S_castling.rook_after.iColumn = present.iColumn - 1; // future.iColumn +1

				bValid = true;
			}
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

bool King::isPathFree(Position startingPos, Position finishingPos, Game& current_game)
{
	bool bFree = false;

	//для рокировки
	if (abs(startingPos.iColumn - finishingPos.iColumn) == 2)
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

	//движение на 1 клетку
	if (!current_game.isSquareOccupied(startingPos.iRow, startingPos.iColumn))
	{
		bFree = true;
	}

	return bFree;
}