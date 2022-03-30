#include "Interface.h"
#include "Game.h"

// ��������� ��������� ��������� ��� �����������(������������ ��������� �������)
std::string next_message;

// ���������������� ���������
// ��� �������, ����������� � ����������������� ����������, ��������� � ���� �������
// �������, ����, �����, ��������� ������������

void createNextMessage(std::string msg)
{
	next_message = msg;
}

void appendToNextMessage(std::string msg)
{
	next_message += msg;
}

void clearScreen()
{
	system("cls");
}

void printLogo()
{
	std::cout << "       _____ _    _ ______  _____ _____" << std::endl;
	std::cout << "      / ____| |  | |  ____|/ ____/ ____|" << std::endl;
	std::cout << "     | |    | |__| | |__  | (___| (___ " << std::endl;
	std::cout << "     | |    |  __  |  __|  \\___ \\\\___ \\ " << std::endl;
	std::cout << "     | |____| |  | | |____ ____) |___) |" << std::endl;
	std::cout << "      \\_____|_|  |_|______|_____/_____/" << std::endl << std::endl;
}

void printMenu()
{
	std::cout << "Commands: (N)ew game   (M)ove \t(S)ave \t(L)oad \t(Q)uit" << std::endl;
}

void printMessage()
{
	std::cout << next_message << std::endl;

	next_message = "";
}

void printLine(int iLine, char iColor1, char iColor2, Game& game)
{
	// (��� ������ = 6 �� �����������):

	//  [6-char]
	//  |______| ��������� 1
	//  |__WQ__| ��������� 2
	//  |______| ��������� 3

	// ���������� ���������� CELL.
	// ��� ������������, ������� �������������� �������� �������� ���� �������
	// ���������� ������������ �������� ����� CELL / 2
	// ����� �������� ���, ����� �������� ������ ����� (��� �������� ����� �������� ����� ��������� ��������������)
	int CELL = 6;

	// ��������� ������ �������� ������� � ������ ���������� �������� ������,
	// ��� ����� ������������ ��� ������� ������.
	// ����, ���� � ��� ���� ������� CELL, � ��� ������ ���� ��������� CELL / 2
	for (int subLine = 0; subLine < CELL / 2; subLine++)
	{
		// ��������� ������� �� 8 �����, �� �� ����� ������������� ��
		// � 4 iPairs of black&white (������� � ������)
		for (int iPair = 0; iPair < 4; iPair++)
		{
			// ������ ������ ����
			for (int subColumn = 0; subColumn < CELL; subColumn++)
			{
				// ������ ������� ���������� ����������� ������
				// ��� 3 �������� � ��������� 1
				// ��� 6 �����������, ���������� 2 -����, 3-������
				if (subLine == 1 && subColumn == 2)
				{
					Piece* temp = game.getPieceAtPosition(iLine, iPair * 2);
					std::cout << (temp != 0 ? temp->GetColor() : char(iColor1));
				}
				else
					if (subLine == 1 && subColumn == 3)
					{
						Piece* temp = game.getPieceAtPosition(iLine, iPair * 2);
						std::cout << (temp != 0 ? temp->GetPiece() : char(iColor1));
					}
					else
					{
						std::cout << char(iColor1);
					}
			}

			// ��������� ������ ����
			for (int subColumn = 0; subColumn < CELL; subColumn++)
			{
				// ������ ������� ���������� ����������� ������
				// ��� 3 �������� � ��������� 1
				// ��� 6 �����������, ���������� 2 -����, 3-������
				if (subLine == 1 && subColumn == 2)
				{
					Piece* temp = game.getPieceAtPosition(iLine, iPair * 2 + 1);
					std::cout << (temp != 0 ? temp->GetColor() : char(iColor2));
				}
				else
					if (subLine == 1 && subColumn == 3)
					{
						Piece* temp = game.getPieceAtPosition(iLine, iPair * 2 + 1);
						std::cout << (temp != 0 ? temp->GetPiece() : char(iColor2));
					}
					else
					{
						std::cout << char(iColor2);
					}
			}
		}
		// ���������� ����� ������ ������
		if (1 == subLine)
		{
			std::cout << "   " << iLine + 1;
		}

		std::cout << std::endl;
	}
}

void printBoard(Game& game)
{
	std::cout << "   A     B     C     D     E     F     G     H\n\n";

	for (int iLine = 7; iLine >= 0; iLine--)
	{
		if (iLine % 2 == 0)
		{
			// ����� ���������� � ������ ������
			printLine(iLine, BLACK_SQUARE, WHITE_SQUARE, game);
		}

		else
		{
			// ����� ���������� � ����� ������
			printLine(iLine, WHITE_SQUARE, BLACK_SQUARE, game);
		}
	}
}

void printSituation(Game& game)
{
	// ��������� ���� - ��������, ������ ���� ��� ������ ���� �� ���� ���
	if (0 != game.rounds.size())
	{
		std::cout << "Last moves:\n";

		int iMoves = game.rounds.size();
		int iToShow = iMoves;		//������� ����� ��� ���� �������

		std::string space = "";

		while (iToShow--)
		{
			if (iMoves < 10)
			{
				// �������� �������������� ������������, ����� ��������� ����� ������ 10
				space = " ";
			}

			std::cout << space << iMoves << " ..... " << game.rounds[iMoves - 1].white_move.c_str() << " | " << game.rounds[iMoves - 1].black_move.c_str() << "\n";
			iMoves--;
		}

		std::cout << "\n";
	}

	// ����������� ������ - ������ ������ � ��� ������, ���� ���� ��������� ���� �� ����� ������
	if (0 != game.white_captured.size() || 0 != game.black_captured.size())
	{
		std::cout << "---------------------------------------------\n";
		std::cout << "WHITE captured: ";
		for (unsigned i = 0; i < game.white_captured.size(); i++)
		{
			std::cout << game.white_captured[i] << " ";
		}
		std::cout << "\n";

		std::cout << "BLACK captured: ";
		for (unsigned i = 0; i < game.black_captured.size(); i++)
		{
			std::cout << game.black_captured[i] << " ";
		}
		std::cout << "\n";

		std::cout << "---------------------------------------------\n";
	}

	// ������� ���
	std::cout << "Current turn: " << (game.getCurrentTurn() == 'w' ? "WHITE " : "BLACK ") << "\n\n";
}
