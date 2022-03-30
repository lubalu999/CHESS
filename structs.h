#pragma once

class Piece;


///Direction
enum Direction			//�����������
{
	HORIZONTAL = 0,
	VERTICAL,
	DIAGONAL,
	L_SHAPE				//��� ����
};

///Position
struct Position			//�������
{
	int iRow;
	int iColumn;
};

///En passant
struct EnPassant		//������ �� �������
{
	bool bApplied;
	Position PawnCaptured;
};

///Castling	
struct Castling			//���������
{
	bool bApplied;
	Position rook_before;		//�������������
	Position rook_after;		//��������������
};

///Promotion of a pawn
struct Promotion			//����������� �����
{
	bool bApplied;
	char chBefore;
	char chAfter;
};

///The intended move
struct IntendedMove			//�������������� ���
{
	Piece* chPiece;			//������
	Position from;
	Position to;
};

///Attacker
struct Attacker				//���������
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