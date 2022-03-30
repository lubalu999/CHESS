#pragma once

class Piece;


///Direction
enum Direction			//направление
{
	HORIZONTAL = 0,
	VERTICAL,
	DIAGONAL,
	L_SHAPE				//для коня
};

///Position
struct Position			//позиция
{
	int iRow;
	int iColumn;
};

///En passant
struct EnPassant		//взятие на проходе
{
	bool bApplied;
	Position PawnCaptured;
};

///Castling	
struct Castling			//рокировка
{
	bool bApplied;
	Position rook_before;		//левосторонняя
	Position rook_after;		//правосторонняя
};

///Promotion of a pawn
struct Promotion			//продвижение пешки
{
	bool bApplied;
	char chBefore;
	char chAfter;
};

///The intended move
struct IntendedMove			//предполагаемый ход
{
	Piece* chPiece;			//фигура
	Position from;
	Position to;
};

///Attacker
struct Attacker				//атакующий
{
	Position  pos;
	Direction dir;
};

///The structure of the cancellation of the move
struct Undo
{
	bool bCanUndo;
	bool bCapturedLastMove;

	bool bCastlingKingSideAllowed;
	bool bCastlingQueenSideAllowed;

	EnPassant en_passant;
	Castling  castling;
	Promotion promotion;
};