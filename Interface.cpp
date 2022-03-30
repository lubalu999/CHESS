#include "Interface.h"
#include "Game.h"

// Сохраняем следующее сообщение для отображения(относительно последней команды)
std::string next_message;

// Пользовательский интерфейс
// Все функции, относящиеся к пользовательскому интерфейсу, находятся в этом разделе
// Логотип, Меню, Доска, сообщения пользователю

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
	// (для клетки = 6 по горизонтали):

	//  [6-char]
	//  |______| подстрока 1
	//  |__WQ__| подстрока 2
	//  |______| подстрока 3

	// Определяем переменную CELL.
	// Оно представляет, сколько горизонтальных символов образуют один квадрат
	// Количество вертикальных символов будет CELL / 2
	// Можно изменить его, чтобы изменить размер доски (при нечетном числе квадраты будут выглядеть прямоугольными)
	int CELL = 6;

	// Поскольку ширина символов ЧЕРНОГО и БЕЛОГО составляет половину высоты,
	// Нам нужно использовать два символа подряд.
	// Итак, если у нас есть символы CELL, у нас должны быть подстроки CELL / 2
	for (int subLine = 0; subLine < CELL / 2; subLine++)
	{
		// Подстрока состоит из 8 ячеек, но мы можем сгруппировать ее
		// в 4 iPairs of black&white (черного и белого)
		for (int iPair = 0; iPair < 4; iPair++)
		{
			// Первая ячейка пары
			for (int subColumn = 0; subColumn < CELL; subColumn++)
			{
				// Фигура должена находиться «посередине» ячейки
				// Для 3 подстрок в подстроке 1
				// Для 6 подстолбцов, подстолбец 2 -цвет, 3-фигура
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

			// Следующая клетка пары
			for (int subColumn = 0; subColumn < CELL; subColumn++)
			{
				// Фигура должена находиться «посередине» ячейки
				// Для 3 подстрок в подстроке 1
				// Для 6 подстолбцов, подстолбец 2 -цвет, 3-фигура
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
		// Записываем номер строки справа
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
			// Линия начинается с ЧЕРНОЙ клетки
			printLine(iLine, BLACK_SQUARE, WHITE_SQUARE, game);
		}

		else
		{
			// Линия начинается с БЕЛОЙ ячейки
			printLine(iLine, WHITE_SQUARE, BLACK_SQUARE, game);
		}
	}
}

void printSituation(Game& game)
{
	// Последние ходы - выводить, только если был сделан хотя бы один ход
	if (0 != game.rounds.size())
	{
		std::cout << "Last moves:\n";

		int iMoves = game.rounds.size();
		int iToShow = iMoves;		//сделать чтобы все ходы выводил

		std::string space = "";

		while (iToShow--)
		{
			if (iMoves < 10)
			{
				// Добавить дополнительное пространство, чтобы выровнять числа меньше 10
				space = " ";
			}

			std::cout << space << iMoves << " ..... " << game.rounds[iMoves - 1].white_move.c_str() << " | " << game.rounds[iMoves - 1].black_move.c_str() << "\n";
			iMoves--;
		}

		std::cout << "\n";
	}

	// Захваченные фигуры - печать только в том случае, если была захвачена хотя бы одина фигура
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

	// Текущий ход
	std::cout << "Current turn: " << (game.getCurrentTurn() == 'w' ? "WHITE " : "BLACK ") << "\n\n";
}
