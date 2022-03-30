#include "Game.h"

Game::Game()
{
	// ����� ������ ��������
	m_CurrentTurn = 'w';

	// ������ ����!
	m_bGameFinished = false;

	// ��� ������ �� ���������
	m_undo.bCapturedLastMove = false;
	m_undo.bCanUndo = false;
	m_undo.bCastlingKingSideAllowed = false;
	m_undo.bCastlingQueenSideAllowed = false;
	m_undo.en_passant.bApplied = false;
	m_undo.castling.bApplied = false;

	Board.resize(8);

	for (int i = 0; i < 8; i++)
	{
		Board[i].resize(8);
	}

	for (int iRow = 0; iRow < 8; ++iRow) 
	{
		for (int iCol = 0; iCol < 8; ++iCol) 
		{
			Board[iRow][iCol] = nullptr;
		}
	}
	for (int iCol = 0; iCol < 8; ++iCol) 
	{
		Board[6][iCol] = new Pawn('b');
	}
	Board[7][0] = new Rook('b');
	Board[7][1] = new Knight('b');
	Board[7][2] = new Bishop('b');
	Board[7][3] = new Queen('b');
	Board[7][4] = new King('b');
	Board[7][5] = new Bishop('b');
	Board[7][6] = new Knight('b');
	Board[7][7] = new Rook('b');

	for (int iCol = 0; iCol < 8; ++iCol) {
		Board[1][iCol] = new Pawn('w');
	}
	Board[0][0] = new Rook('w');
	Board[0][1] = new Knight('w');
	Board[0][2] = new Bishop('w');
	Board[0][3] = new Queen('w');
	Board[0][4] = new King('w');
	Board[0][5] = new Bishop('w');
	Board[0][6] = new Knight('w');
	Board[0][7] = new Rook('w');
}

Game::~Game()
{
	white_captured.clear();
	black_captured.clear();
	rounds.clear();
}

Piece* Game::getPieceAtPosition(int iRow, int iColumn) const
{
	return Board[iRow][iColumn];
}

bool Game::isSquareOccupied(int iRow, int iColumn) const
{
	bool bOccupied = false;

	if (getPieceAtPosition(iRow, iColumn) != 0)
	{
		bOccupied = true;
	}

	return bOccupied;
}

bool Game::isFinished() const
{
	return m_bGameFinished;
}

char Game::getCurrentTurn() const
{
	return m_CurrentTurn;
}

bool Game::undoIsPossible() const
{
	return m_undo.bCanUndo;
}

bool Game::CheckCell(int iDestRow, int iDestCol) const
{
	return (iDestRow >= 0 && iDestRow < 8 && iDestCol >= 0 && iDestCol < 8);
}

std::string Game::getLastMove() const
{
	std::string last_move;

	if (getCurrentTurn() == 'b')			// ��� ������ ��������� ���?
	{
		last_move = rounds[rounds.size() - 1].white_move;			//���� ������ ������� ������, � ����� ��� ��������� ���
	}
	else
	{
		last_move = rounds[rounds.size() - 1].black_move;			// ��������� ��� ��� ������
	}

	return last_move;
}

void Game::logMove(std::string &to_record)
{
	// ���� ������ �������� ����� 5 �������� � �������
	if (to_record.length() == 5)
	{
		to_record += "  ";
	}

	if (getCurrentTurn() == 'w')
	{
		// ���� ��� ��� ��� ������ ������, ������� ����� ����� � ��������� ���� black_move ������.
		Round round;
		round.white_move = to_record;
		round.black_move = "";

		rounds.push_back(round);
	}
	else
	{
		// ���� ��� ��� ��� ������, ������ ��������� Round
		Round round = rounds[rounds.size() - 1];
		round.black_move = to_record;

		//������� ��������� ����� � ������� ��� �������, �� ������ ������ �����
		rounds.pop_back();
		rounds.push_back(round);
	}
}

void Game::parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted)
{
	pFrom->iColumn = move[0];
	pFrom->iRow = move[1];
	pTo->iColumn = move[3];
	pTo->iRow = move[4];

	// ������������� ������� �� ['A'-'H'] � [0x00-0x07]
	pFrom->iColumn = pFrom->iColumn - 'A';
	pTo->iColumn = pTo->iColumn - 'A';

	// ������������� ������ �� ['1'-'8'] � [0x00-0x07]
	pFrom->iRow = pFrom->iRow - '1';
	pTo->iRow = pTo->iRow - '1';

	if (chPromoted != nullptr)
	{
		if (move[5] == '=')
		{
			*chPromoted = move[6];
		}
		else
		{
			*chPromoted = 0;
		}
	}
}

bool Game::playerKingInCheck(IntendedMove* intended_move)
{
	return isKingInCheck(getCurrentTurn(), intended_move);
}


Position Game::findKing(char iColor) const
{
	Position king = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* figure = getPieceAtPosition(i, j);

			if (figure == nullptr)
			{
				//������ �����
				continue;
			}

			if (figure->GetPiece() == 'K' && figure->GetColor() == iColor)
			{
				king.iRow = i;
				king.iColumn = j;
			}
		}
	}

	return king;
}

void Game::makeMove(Position present, Position future)
{
	// �������� ������ �� �������
	Piece* figure = getPieceAtPosition(present.iRow, present.iColumn);


	// ������ ���������� ������?
	Piece* capturedPiece = getPieceAtPosition(future.iRow, future.iColumn);

	if (capturedPiece)
	{
		if (getCurrentTurn() == 'w' && capturedPiece->isBlackPiece())			
		{
			// ����� ������ ���� ���������
			white_captured.push_back(capturedPiece->GetPiece());
		}
		else
		{
			// ������ ������ ���� ���������
			black_captured.push_back(capturedPiece->GetPiece());
		}

		// ���������� ��������� ������. ���� ������ ���� �����, �� ��� ��� ������� �� �����������.
		m_undo.bCapturedLastMove = true;
	}
	else
	{
		m_undo.bCapturedLastMove = false;
	}

	// ������ ������ �� �������� ���������
	Board[present.iRow][present.iColumn] = nullptr;
	Board[future.iRow][future.iColumn] = figure;

	// �������� ���
	changeTurns();

	// ���� ��� ����� ��������
	m_undo.bCanUndo = true;

	// ��������, ��������� �� �� ���� ����� ��� ��� ������ ��������� 
	// ����� � ����, ��� ��� ������ ��� ���������
	if (true == playerKingInCheck())
	{
		if (true == isCheckMate())
		{
			if (getCurrentTurn() == 'w')
			{
				std::cout<< "Checkmate! Black wins the game!\n";
			}
			else
			{
				std::cout << "Checkmate! White wins the game!\n";
			}
		}
		else
		{
			if (getCurrentTurn() == 'w')
			{
				std::cout << "White king is in check!\n";
			}
			else
			{
				std::cout << "Black king is in check!\n";
			}
		}
	}
}

void Game::changeTurns()
{
	if (getCurrentTurn() == 'w')
		m_CurrentTurn = 'b';
	else
		m_CurrentTurn = 'w';
}

bool Game::isKingInCheck(char iColor, IntendedMove* pintended_move) 			//iColor - ���� ������
{
	bool bCheck = false;

	Position king = { 0 };

	// ���������� ���������, �������� �� �������������� ��� ����� �������
	if (nullptr != pintended_move && 'K' == pintended_move->chPiece->GetPiece())
	{
		king.iRow = pintended_move->to.iRow;
		king.iColumn = pintended_move->to.iColumn;
	}
	else
	{
		king = findKing(iColor);
	}

	UnderAttack king_attacked = isUnderAttack(king.iRow, king.iColumn, iColor, pintended_move);

	if (true == king_attacked.bUnderAttack)
	{
		bCheck = true;
	}

	return bCheck;
}

UnderAttack Game::isUnderAttack(int iRow, int iColumn, char iColor, IntendedMove* pintended_move)		//iColor - ���� �������� ������
{
	UnderAttack attack = { 0 };

	// a) �����������: HORIZONTAL
	{
		// ��������� �� �� ����� ������
		for (int i = iColumn + 1; i < 8; i++)
		{
			Piece* pieceFound = getPiece_considerMove(iRow, i, pintended_move);

			if (pieceFound == nullptr)
			{
				// ��� ������ �����, ���������� ������
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ��� ������ ���� �� �����, ������� ��� �������
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ������ ����� ��� �����, ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRow;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = i;
				attack.attacker[attack.iNumAttackers - 1].dir = HORIZONTAL;			//?
				break;
			}
			else
			{
				// ���� ������, ������� �� ������� �� �����������, ��� ��� ��� �������
				break;
			}
		}

		// ��������� �� �����
		for (int i = iColumn - 1; i >= 0; i--)
		{
			Piece* pieceFound = getPiece_considerMove(iRow, i, pintended_move);

			if (pieceFound == nullptr)
			{
				// ��� ������ �����, ���������� ������
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ��� ������ ���� �� �����, ������� ��� �������
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ����� ��� �����, ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRow;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = i;
				attack.attacker[attack.iNumAttackers - 1].dir = HORIZONTAL;
				break;
			}
			else
			{
				// ���� ������, ������� �� ������� �� �����������, ��� ��� ��� �������
				break;
			}
		}
	}

	// b) �����������: VERTICAL
	{
		// ��������� ��������� �����
		for (int i = iRow + 1; i < 8; i++)
		{
			Piece* pieceFound = getPiece_considerMove(i, iColumn, pintended_move);

			if (pieceFound == nullptr)
			{
				// ��� ������ �����, ���������� ������
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ��� ������ ���� �� �����, ������� ��� �������
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ��� �������� ������, ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumn;
				attack.attacker[attack.iNumAttackers - 1].dir = VERTICAL;
				break;
			}
			else
			{
				// ��� ������ �� ������� �� ���������, �� ������
				break;
			}
		}

		// ��������� �� ����
		for (int i = iRow - 1; i >= 0; i--)
		{
			Piece* pieceFound = getPiece_considerMove(i, iColumn, pintended_move);

			if (pieceFound == nullptr)
			{
				// ��� ������ �����, ���������� ������
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ��� ������ ���� �� �����, ������� ��� �������
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ��� �������� ������, ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumn;
				attack.attacker[attack.iNumAttackers - 1].dir = VERTICAL;
				break;
			}
			else
			{
				// ��� ������ �� ������� �� ���������, �� ������
				break;
			}
		}
	}

	// c) �����������: DIAGONAL
	{
		// ��������� ��������� �����-������
		for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
		{
			Piece* pieceFound = getPiece_considerMove(i, j, pintended_move);

			if (pieceFound == nullptr)
			{
				// ������ �����
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ������ ������ �����, ��� ������
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && (i == iRow + 1) && (j == iColumn + 1) && iColor == 'w')
			{
				// ����� ������ ��� ������ ������ ������, ������ ���� ��� (�� ���������) ��������� ����� � ���
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				// ���� ���� ��� �����, �� ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else
			{
				// ��� ������ �� �������, ��� ������
				break;
			}
		}

		// ��������� ��������� �����-�����
		for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
		{
			Piece* pieceFound = getPiece_considerMove(i, j, pintended_move);

			if (pieceFound == nullptr)
			{
				// ������ �����
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ������ ������ �����, ��� ������
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && (i == iRow + 1) && (j == iColumn + 1) && iColor == 'w')
			{
				// ����� ������ ��� ������ ������ ������, ������ ���� ��� (�� ���������) ��������� ����� � ���
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				// ���� ���� ��� �����, �� ������ � ���������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else
			{
				// ��� ������ �� �������, ��� ������
				break;
			}
		}

		// ��������� ��������� ����-������
		for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
		{
			Piece* pieceFound = getPiece_considerMove(i, j, pintended_move);

			if (pieceFound == nullptr)
			{
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && (i == iRow - 1) && (j == iColumn + 1) && iColor == 'b')
			{
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				// ���� ����� ��� ����, ������ � ����������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else
			{
				// ������ ������, ������� ���
				break;
			}
		}

		// ��������� ��������� ����-�����
		for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
		{
			Piece* pieceFound = getPiece_considerMove(i, j, pintended_move);

			if (pieceFound == nullptr)
			{
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && (i == iRow - 1) && (j == iColumn - 1) && iColor == 'b')
			{
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				// ���� ����� ��� ����, ������ � ����������
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = i;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = j;
				attack.attacker[attack.iNumAttackers - 1].dir = DIAGONAL;
				break;
			}
			else
			{
				// ������ ������, ������� ���
				break;
			}
		}
	}

	// d) �����������: L_SHAPED
	{
		// ���������, �� ����������� �� ������ ��������� �� ����
		Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
									 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
		for (int i = 0; i < 8; i++)
		{
			int iRowToTest = iRow + knight_moves[i].iRow;
			int iColumnToTest = iColumn + knight_moves[i].iColumn;

			if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
			{
				// ����� �������� �� ����������, ������� ��������� �� �����
				continue;
			}

			Piece* pieceFound = getPiece_considerMove(iRowToTest, iColumnToTest, pintended_move);
			if (pieceFound == nullptr)
			{
				// ������ �����
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				// ��� ������ ������ �����
				continue;
			}
			else if (pieceFound->GetPiece() == 'N')
			{
				attack.bUnderAttack = true;
				attack.iNumAttackers += 1;

				attack.attacker[attack.iNumAttackers - 1].pos.iRow = iRowToTest;
				attack.attacker[attack.iNumAttackers - 1].pos.iColumn = iColumnToTest;
				break;
			}
		}
	}

	return attack;
}

Piece* Game::getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move)
{
	Piece* figure = nullptr;

	// ���� ����������� ���� ���, ������ ������ ������� ��������� �����
	if (nullptr == intended_move)
	{
		figure = getPieceAtPosition(iRow, iColumn);
	}
	else
	{
		// � ���� ������ �� �������� ������, ��� ���� ��, ���� �� ���� ��� ��� ������, 
		// ������� �� ������������� ���, ������� ��� �� ��� ������
		if (intended_move->from.iRow == iRow && intended_move->from.iColumn == iColumn)
		{
			// ������ ����� ����� �� ����� ����, ������� ��� ����� ������
			figure = nullptr;
		}
		else if (intended_move->to.iRow == iRow && intended_move->to.iColumn == iColumn)
		{
			// ������ ����� ������������� �� ��� ����, ������� ������ ������
			figure = intended_move->chPiece;
		}
		else
		{
			figure = getPieceAtPosition(iRow, iColumn);
		}
	}

	return figure;
}

bool Game::wouldKingBeInCheck(Piece* chPiece, Position present, Position future, EnPassant* S_enPassant)
{
	IntendedMove intended_move;

	intended_move.chPiece = chPiece;
	intended_move.from.iRow = present.iRow;
	intended_move.from.iColumn = present.iColumn;
	intended_move.to.iRow = future.iRow;
	intended_move.to.iColumn = future.iColumn;

	return playerKingInCheck(&intended_move);
}

char Game::getOpponentColor() const
{
	if (getCurrentTurn() == 'w')
	{
		return 'b';
	}
	else
	{
		return 'w';
	}
}

void Game::deleteLastMove()
{
	if (getCurrentTurn() == 'w')
	{
		// ��������� ����� ���� ������� �����, ��� ��� ������ pop_back()
		rounds.pop_back();
	}
	else
	{
		// ��������� ��� ��� ������
		Round round = rounds[rounds.size() - 1];
		round.black_move = "";

		// �������� ��������� ����� � ������� ���, ������ ��� ������� ����
		rounds.pop_back();
		rounds.push_back(round);
	}
}

bool Game::isReachable(int iRow, int iColumn, char iColor)
{
	bool bReachable = false;

	// a) ����������: HORIZONTAL
	{
		//��������� �� ������
		for (int i = iColumn + 1; i < 8; i++)
		{
			Piece* pieceFound = getPieceAtPosition(iRow, i);

			if (pieceFound == nullptr)
			{
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ������ ����� ��� �����, ������� ���� ��������
				bReachable = true;
				break;
			}
			else
			{
				// ���� ������, ������� �� �������� �� �����������
				break;
			}
		}

		// ��������� �� �����
		for (int i = iColumn - 1; i >= 0; i--)
		{
			Piece* pieceFound = getPieceAtPosition(iRow, i);

			if (pieceFound == nullptr)
			{
				continue;
			}

			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ����� ��� �����, ������� ���� ��������
				bReachable = true;
				break;
			}
			else
			{
				// ���� ������, ������� �� �������� �� �����������
				break;
			}
		}
	}

	// b) �����������: VERTICAL
	{
		// ��������� �������� �����
		for (int i = iRow + 1; i < 8; i++)
		{
			Piece* pieceFound = getPieceAtPosition(i, iColumn);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && pieceFound->GetColor() == 'b' && i == iRow + 1)
			{
				//�� ���� ������ ����� �����, ������, ���� ���������
				bReachable = true;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ��� �����, ������� ���� ��������
				bReachable = true;
				break; 
			}
			else
			{
				// ���� ������, ������� �� ��������� �� ���������
				break;
			}
		}

		// ��������� �� ����
		for (int i = iRow - 1; i >= 0; i--)
		{
			Piece* pieceFound = getPieceAtPosition(i, iColumn);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'P' && pieceFound->GetColor() == 'w' && i == iRow - 1)
			{
				bReachable = true;
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'R')
			{
				// ����� ��� �����, ������� ���� ��������
				bReachable = true;
				break;
			}
			else
			{
				// ���� ������, ������� �� ��������� �� ���������
				break;
			}
		}
	}

	// c) �����������: DIAGONAL
	{
		// �����-������
		for (int i = iRow + 1, j = iColumn + 1; i < 8 && j < 8; i++, j++)
		{
			Piece* pieceFound = getPieceAtPosition(i, j);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				bReachable = true;
				break;
			}
			else
			{
				break;
			}
		}

		// �����-�����
		for (int i = iRow + 1, j = iColumn - 1; i < 8 && j > 0; i++, j--)
		{
			Piece* pieceFound = getPieceAtPosition(i, j);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				bReachable = true;
				break;
			}
			else
			{
				break;
			}
		}

		// ����-������
		for (int i = iRow - 1, j = iColumn + 1; i > 0 && j < 8; i--, j++)
		{
			Piece* pieceFound = getPieceAtPosition(i, j);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				bReachable = true;
				break;
			}
			else
			{
				break;
			}
		}

		// ����-�����
		for (int i = iRow - 1, j = iColumn - 1; i > 0 && j > 0; i--, j--)
		{
			Piece* pieceFound = getPieceAtPosition(i, j);

			if (pieceFound == nullptr)
			{
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				break;
			}
			else if (pieceFound->GetPiece() == 'Q' || pieceFound->GetPiece() == 'B')
			{
				bReachable = true;
				break;
			}
			else
			{
				break;
			}
		}
	}

	// d) �����������: L_SHAPED
	{
		// ���������, �� ����������� �� ������ ��������� �� ������

		Position knight_moves[8] = { {  1, -2 }, {  2, -1 }, {  2, 1 }, {  1, 2 },
									 { -1, -2 }, { -2, -1 }, { -2, 1 }, { -1, 2 } };
		for (int i = 0; i < 8; i++)
		{
			int iRowToTest = iRow + knight_moves[i].iRow;
			int iColumnToTest = iColumn + knight_moves[i].iColumn;

			if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
			{
				// ������ �� ����������
				continue;
			}

			Piece* pieceFound = getPieceAtPosition(iRowToTest, iColumnToTest);

			if (pieceFound == nullptr)
			{
				//������ �����
				continue;
			}
			if (iColor == pieceFound->GetColor())
			{
				//��� ����
				break;
			}
			else if (pieceFound->GetPiece() == 'N')
			{
				bReachable = true;
				break;
			}
		}
	}

	return bReachable;
}

bool Game::canBeBlocked(Position startingPos, Position finishingPos, int iDirection)
{
	bool bBlocked = false;

	UnderAttack blocker = { 0 };

	switch (iDirection)
	{
	case HORIZONTAL:
	{
		// ���� ��� �������������� �����������, �� ����� ������������, ��� startPos.iRow == finishPos.iRow
		// ���� ������ ����� ������������� �� ������� 0 � ������� 7, �� ������ ���������, �������� �� ������� 1-6.
		if (startingPos.iColumn == finishingPos.iColumn)
		{
			std::cout << "Error. Movement is horizontal but column is the same\n";
		}

		// �������� ������
		else if (startingPos.iColumn < finishingPos.iColumn)
		{
			for (int i = startingPos.iColumn + 1; i < finishingPos.iColumn; i++)
			{
				if (isReachable(startingPos.iRow, i, getOpponentColor()) == true)
				{
					// �����-�� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}

		// �������� �����
		else //if (startingPos.iColumn > finishingPos.iColumn)
		{
			for (int i = startingPos.iColumn - 1; i > finishingPos.iColumn; i--)
			{
				if (true == isReachable(startingPos.iRow, i, getOpponentColor()))
				{
					// �����-�� ������ ����� ������������� ����
					bBlocked = true;
				}
			}
		}
	}
	break;

	case VERTICAL:
	{
		// ���� ��� ������������ �����������, �� ����� ������������, ��� startPos.iRow == finishPos.iRow
		// ���� ������ ����� ������������� �� ������ 0 � ������ 7, �� ������ ���������, �������� �� ������ 1-6
		if (startingPos.iRow == finishingPos.iRow)
		{
			std::cout << "Error. Movement is vertical but row is the same\n";
			throw("Error. Movement is vertical but row is the same");
		}

		// �����
		else if (startingPos.iRow < finishingPos.iRow)
		{
			for (int i = startingPos.iRow + 1; i < finishingPos.iRow; i++)
			{
				if (true == isReachable(i, startingPos.iColumn, getOpponentColor()))
				{
					// ����� �������������
					bBlocked = true;
				}
			}
		}

		// ����
		else //if (startingPos.iColumn > finishingPos.iRow)
		{
			for (int i = startingPos.iRow - 1; i > finishingPos.iRow; i--)
			{
				if (true == isReachable(i, startingPos.iColumn, getOpponentColor()))
				{
					// ��������� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}
	}
	break;

	case DIAGONAL:
	{
		// ����� ������
		if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
		{
			for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
			{
				if (true == isReachable(startingPos.iRow + i, startingPos.iColumn + i, getOpponentColor()))
				{
					// ��������� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}

		// �����-�����
		else if ((finishingPos.iRow > startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
		{
			for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
			{
				if (true == isReachable(startingPos.iRow + i, startingPos.iColumn - i, getOpponentColor()))
				{
					// ��������� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}

		// ����-������
		else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn > startingPos.iColumn))
		{
			for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
			{
				if (true == isReachable(startingPos.iRow - i, startingPos.iColumn + i, getOpponentColor()))
				{
					// ��������� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}

		// ����-�����
		else if ((finishingPos.iRow < startingPos.iRow) && (finishingPos.iColumn < startingPos.iColumn))
		{
			for (int i = 1; i < abs(finishingPos.iRow - startingPos.iRow); i++)
			{
				if (true == isReachable(startingPos.iRow - i, startingPos.iColumn - i, getOpponentColor()))
				{
					// ��������� ������ ����� ���������� ����
					bBlocked = true;
				}
			}
		}

		else
		{
			std::cout << "Error. Diagonal move not allowed\n";
			throw("Error. Diagonal move not allowed");
		}
	}
	break;
	}

	return bBlocked;
}

bool Game::isCheckMate()
{
	bool bCheckmate = false;

	// 1. ��� ������, ������ ��� �����?
	if (playerKingInCheck() == false)
	{
		return false;
	}

	// 2. ����� �� ������ ������������� � ������ ������?
	Position king_moves[8] = { {  1, -1 },{  1, 0 },{  1,  1 }, { 0,  1 },
									   { -1,  1 },{ -1, 0 },{ -1, -1 }, { 0, -1 } };
	
	Position king = findKing(getCurrentTurn());
	for (int i = 0; i < 8; i++)
	{
		int iRowToTest = king.iRow + king_moves[i].iRow;
		int iColumnToTest = king.iColumn + king_moves[i].iColumn;

		if (iRowToTest < 0 || iRowToTest > 7 || iColumnToTest < 0 || iColumnToTest > 7)
		{
			// ������ �������� ���� �� ����������, ������� ��������� �� �����
			continue;
		}

		if (getPieceAtPosition(iRowToTest, iColumnToTest) != nullptr)
		{
			// ���� ������� �� ����, ������� ��������� �� �����
			continue;
		}

		IntendedMove intended_move;
		intended_move.chPiece = getPieceAtPosition(king.iRow, king.iColumn);
		intended_move.from.iRow = king.iRow;
		intended_move.from.iColumn = king.iColumn;
		intended_move.to.iRow = iRowToTest;
		intended_move.to.iColumn = iColumnToTest;

		// ������ ��� ������� ���������� ���� ������ ���������, �� �������� �� �� ��� �����
		UnderAttack king_moved = isUnderAttack(iRowToTest, iColumnToTest, getCurrentTurn(), &intended_move);
		if (king_moved.bUnderAttack == false)
		{
			// ��� ��������, ��� ���� ���� �� ���� �������, � ������� ������ �� �������� ��� �������, ��� ��� ��� �� ���
			return false;
		}
	}

	// 3. ����� �� �������� ����������� ��� �� ��� ���� ������� ������ ������? 
	UnderAttack king_attacked = isUnderAttack(king.iRow, king.iColumn, getCurrentTurn());
	if (king_attacked.iNumAttackers == 1)
	{
		UnderAttack king_attacker = { 0 };
		king_attacker = isUnderAttack(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn, getOpponentColor());

		if (king_attacker.bUnderAttack == true)
		{
			// ��� ������, ��� ����������� ����� ��������, ��� ��� ��� �� ���
			return false;
		}
		else
		{
			// � ������� ������: ����� �� �����-������ ������ ������ ����� ���������� � �������?
			Piece* attacker = getPieceAtPosition(king_attacked.attacker[0].pos.iRow, king_attacked.attacker[0].pos.iColumn);

			switch (attacker->GetPiece())
			{
			case 'P':
			case 'N':
			{
				// ���� ��� �����, ����� ��������� � ������� ��� �����
				// ���� ��� ����, ��� �� ����� ��������, ������ ��� ���� ����� "�������"
				// ����� �������, ���
				bCheckmate = true;
			}
			break;

			case 'B':
			{
				if (canBeBlocked(king_attacked.attacker[0].pos, king, DIAGONAL) == false)
				{
					// ���� ������� ������ �� ����� ������ �� ����, �������� ���
					bCheckmate = true;
				}
			}
			break;

			case 'R':
			{
				if (canBeBlocked(king_attacked.attacker[0].pos, king, king_attacked.attacker[0].dir) == false)
				{
					// ���� ������� ������ �� ����� ������ �� ����, �������� ���
					bCheckmate = true;
				}
			}
			break;

			case 'Q':
			{
				if (canBeBlocked(king_attacked.attacker[0].pos, king, king_attacked.attacker[0].dir) == false)
				{
					// ���� ������� ������ �� ����� ������ �� ����, �������� ���
					bCheckmate = true;
				}
			}
			break;

			default:
			{
				throw("!!!!Should not reach here. Invalid piece");
			}
			break;
			}
		}
	}

	else
	{
		// ���� ���������� ������ ������ ��� �� ����� �� ���� �����, ��� ���
		bCheckmate = true;
	}

	// ���� ���� �����������, �������� � ���������� ������
	m_bGameFinished = bCheckmate;

	return bCheckmate;
}