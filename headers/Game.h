#pragma once
#include "King.h"
#include "Bishop.h"
#include "Pawn.h"
#include "Knight.h"
#include "Queen.h"
#include "Rook.h"
#include "includes.h"
#include "Vector.h"

/*!
 * \brief	Main class, include in yourself all pieces and их основное взаимолействие
 */

class Game
{
public:
	Game();
	~Game();

	Piece* getPieceAtPosition(int iRow, int iColumn) const;
	bool isSquareOccupied(int iRow, int iColumn) const;
	bool CheckCell(int iDestRow, int iDestCol) const;
	void logMove(std::string &to_record);
	void makeMove(Position present, Position future);
	void changeTurns();
	void deleteLastMove();

	struct Round							// Сохранить все ходы
	{
		std::string white_move;
		std::string black_move;
	};
	
	std::deque<Round> rounds;				//std::deque<std::string> moves;

	my_vec<char> white_captured;		// Сохранить убитые фигуры
	my_vec<char> black_captured;

	bool wouldKingBeInCheck(Piece* chPiece, Position present, Position future, EnPassant* S_enPassant);
	bool playerKingInCheck(IntendedMove* intended_move = nullptr);
	bool isKingInCheck(char iColor, IntendedMove* pintended_move = nullptr);
	bool isCheckMate();
	bool canBeBlocked(Position startingPos, Position finishingPos, int iDirection);
	bool isReachable(int iRow, int iColumn, char iColor);
	UnderAttack isUnderAttack(int iRow, int iColumn, char iColor, IntendedMove* pintended_move = nullptr);
	Piece* getPiece_considerMove(int iRow, int iColumn, IntendedMove* intended_move = nullptr);
	
	char getOpponentColor() const;
	bool isFinished() const;
	char getCurrentTurn() const;
	bool undoIsPossible() const;
	std::string getLastMove() const;
	void parseMove(std::string move, Position* pFrom, Position* pTo, char* chPromoted = nullptr);
	Position findKing(char iColor) const;		//найти короля по цвету игрока
	

	EnPassant S_enPassant = { 0 };			//нужны для совершения хода
	Castling S_castling = { 0 };
	Promotion S_promotion = { 0 };

public:			//потом заменить на приват		
	my_vec<my_vec<Piece*>> Board;

	Undo m_undo;
	char  m_CurrentTurn;			// Держать текущий ход
	bool m_bGameFinished;		// Игра завершена?
};