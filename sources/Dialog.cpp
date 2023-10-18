#define _CRT_SECURE_NO_WARNINGS

#include "../headers/Dialog.h"
#include "../headers/Game.h"
#include "../headers/includes.h"
#include "../headers/Interface.h"
#include <filesystem>

using namespace std;

Game* current_game;

void Dialog()
{
	bool bRun = true;

	// �������� ����� � ����������� �������

	clearScreen();
	printLogo();

	string input = "";

	while (bRun)
	{
		printMessage();
		printMenu();

		// ����� ������������
		cout << "Type here: ";
		getline(cin, input);

		if (input.length() != 1)
		{
			cout << "Invalid option. Type one letter only\n\n";
			continue;
		}

		try
		{
			switch (toupper(input[0]))
			{
				case 'N':
				{
					newGame();
					clearScreen();
					printLogo();
					printSituation(*current_game);
					printBoard(*current_game);
				}
				break;

				case 'M':
				{
					if (NULL != current_game)
					{
						if (current_game->isFinished())
						{
							cout << "This game has already finished!\n";
						}
						else
						{
							movePiece();
							printSituation(*current_game);
							printBoard(*current_game);
						}
					}
					else
					{
						cout << "No game running!\n";
					}

				}
				break;

				case 'S':
				{
					if (NULL != current_game)
					{
						saveGame();
						clearScreen();
						printSituation(*current_game);
						printBoard(*current_game);
					}
					else
					{
						cout << "No game running\n";
					}
				}
				break;

				case 'L':
				{
					loadGame();
					clearScreen();
					printSituation(*current_game);
					printBoard(*current_game);
				}
				break;

				case 'Q':
				{
					bRun = false;
				}
				break;

				default:
				{
					cout << "Option does not exist\n\n";
				}
				break;
			}
		}
		catch (const exception& e)
		{
			cout << e.what();
		}
	}
}

void newGame()
{
	if (NULL != current_game)
	{
		delete current_game;
	}
	current_game = new Game();
}

void movePiece()
{
	std::string to_record;

	// ������������ ������ ������, �� ������� ����� ��������� (������� �������)
	cout << "Choose piece to be moved. (example: A1 or b2): ";

	std::string move_from;
	getline(cin, move_from);

	if (move_from.length() > 2)
	{
		createNextMessage("You should type only two characters (column and row)\n");
		return;
	}

	Position present;
	present.iColumn = toupper(move_from[0]) - 'A';		// ����������� ['A'-'H'] � [0x00-0x07]
	present.iRow = move_from[1] - '1';			// ����������� ['1'-'8'] � [0x00-0x07]

	// ������ �� ������������ ���������� ��������?
	// ����� ���������:
	// - ��� ������ ����� (A1-H8)
	// - � ��������� ������ ������ 
	// - ��� ������ ������ ������? (��� ���������)

	if (current_game->CheckCell(present.iColumn, present.iRow) == 0)
	{
		createNextMessage("Invalid cell.\n");
		return;
	}

	// ������� ���
	to_record += present.iColumn + 'A';
	to_record += present.iRow + '1';
	to_record += "-";


	Piece* figure = current_game->getPieceAtPosition(present.iRow, present.iColumn);

	if (figure == nullptr)
	{
		createNextMessage("You picked an EMPTY square.\n");
		return;
	}

	char chPiece = figure->GetPiece();
	cout << "Piece is " << chPiece << std::endl;
	
	if (current_game->getCurrentTurn() != figure->GetColor())			//���� ��������� ������ �� ��������� � ������ ������
	{
		createNextMessage("Selected piece of wrong color!");
		return;
	}

	// �������� ��������� ������
	cout << "Move to: ";
	std::string move_to;
	getline(cin, move_to);

	if (move_to.length() > 2)
	{
		createNextMessage("You should type only two characters (column and row)\n");
		return;
	}

	// ������������ ��� �������� ���?
	// ���� ���������:
	// - ������ ������ ����� (A1-H8)
	// - ��� ����������
	Position future;
	future.iColumn = toupper(move_to[0]) - 'A';
	future.iRow = move_to[1] - '1';

	if (current_game->CheckCell(future.iColumn, future.iRow) == 0 || !figure->isValidMove(present, future, *current_game))
	{
		createNextMessage("Invalid cell.\n");
		return;
	}

	to_record += future.iColumn + 'A';
	to_record += future.iRow + '1';

	// �������� �� �� �� ����� ������
	if (future.iRow == present.iRow && future.iColumn == present.iColumn)
	{
		createNextMessage("[Invalid] You picked the same square!\n");
		return;
	}

	// �������� ���
	current_game->logMove(to_record);

	// ������� ���
	// ������� ������ ������?
	if (current_game->isSquareOccupied(future.iRow, future.iColumn))
	{
		Piece* AuxFigure = current_game->getPieceAtPosition(future.iRow, future.iColumn);

		if (figure->GetColor() != AuxFigure->GetColor())
		{
			createNextMessage(AuxFigure->describePiece() + " captured!\n");
		}
		else
		{
			cout << "Error. We should not be making this move\n";
			throw("Error. We should not be making this move");
		}
	}

	//�������� ���.�������� ���������/����/�����
	current_game->makeMove(present, future);
}

void saveGame()
{
	string fileName;
	cout << "Type file name to be saved (no extension): ";

	getline(cin, fileName);
	fileName += ".dat";

	filesystem::path pathToGame("games/" + fileName);
	fstream ofs(pathToGame.c_str());

	if (ofs.is_open())
	{
		// ������� ���� � ����� ����������
		auto time_now = std::chrono::system_clock::now();
		std::time_t end_time = std::chrono::system_clock::to_time_t(time_now);
		ofs << "[Chess console] Saved at: " << std::ctime(&end_time);

		// ������� ����
		for (unsigned i = 0; i < current_game->rounds.size(); i++)
		{
			ofs << current_game->rounds[i].white_move.c_str() << " | " << current_game->rounds[i].black_move.c_str() << "\n";
		}

		ofs.close();
		createNextMessage("Game saved as " + fileName + "\n");
	}
	else
	{
		cout << "Error creating file! Save failed\n";
	}

	return;
}

void loadGame()
{
	string fileName;
	cout << "Type file name to be loaded (no extension): ";

	getline(cin, fileName);
	fileName += ".dat";

	filesystem::path pathToGame("games/" + fileName);
	ifstream ifs(pathToGame.c_str());

	if (ifs)
	{
		// ������� �������� ������� ����
		if (current_game != nullptr)
		{
			delete current_game;
		}
		current_game = new Game();

		// �����, ������ ������ �� ����� � ������� ����
		std::string line;

		while (std::getline(ifs, line))
		{
			// �������� ����� ������� ���������� � "[]"
			if (0 == line.compare(0, 1, "["))
			{
				continue;
			}

			// � ����� ����� ���� ���� ��� ��� ����
			string loaded_move[2];

			// ����� ����������� � ������� �������
			std::size_t separator = line.find(" |");

			// ��� ������� ���� ������ �� ������ ������ �� �����������
			loaded_move[0] = line.substr(0, separator);

			// ��� ������� ���� ������� �� ����������� �� ����� ������ 
			loaded_move[1] = line.substr(line.find("|") + 2);

			for (int i = 0; i < 2 && loaded_move[i] != ""; i++)
			{
				// ������������� ������
				Position from;
				Position to;

				char chPromoted = 0;

				current_game->parseMove(loaded_move[i], &from, &to, &chPromoted);

				// ��������� ���������� ������
				if (from.iColumn < 0 || from.iColumn > 7 ||
					from.iRow < 0 || from.iRow    > 7 ||
					to.iColumn < 0 || to.iColumn   > 7 ||
					to.iRow < 0 || to.iRow      > 7)
				{
					createNextMessage("[Invalid] Can't load this game because there are invalid lines!\n");

					// �������� �� � ���������
					current_game = new Game();
					return;
				}
				else
				{
					// �������������� ���
					current_game->logMove(loaded_move[i]);

					// ������� ���
					current_game->makeMove(from, to);
				}
			}
		}

		// �������������� ������ ����� ����� ������������
		createNextMessage("Game loaded from " + fileName + "\n");

		return;
	}
	else
	{
		createNextMessage("Error loading " + fileName + ". Creating a new game instead\n");
		current_game = new Game();
	}
}