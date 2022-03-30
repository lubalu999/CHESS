#pragma once
#include <string>
#define WHITE_SQUARE 0xDB
#define BLACK_SQUARE 0xFF
#define EMPTY_SQUARE 0x20

class Game;

void createNextMessage(std::string msg);
void appendToNextMessage(std::string msg);
void clearScreen();
void printLogo();
void printMenu();
void printMessage();
void printLine(int iLine, char iColor1, char iColor2, Game& game);
void printSituation(Game& game);
void printBoard(Game& game);