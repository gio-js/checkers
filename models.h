#include "const.h"

/**
 * Defines the pawn structure
 */
typedef struct Pawn {
	enum PAWN_TYPE PawnType; // simple 
	enum PLAYER_TYPE PlayerType;
} t_Pawn;

/*
 * Defines a checkers cell structure
 */
typedef struct Cell {

	t_Pawn *Pawn; // null if this is case of empty cell, otherwise pointing to a valid mem element
	enum CELL_TYPE CellType; // white|black
	int X; // from left to right, starting from 0 to 15
	int Y; // from top to bottom, starting from 0 to 15

} t_Cell;

/*
 * Defines the checkers table
 */
typedef struct Table {

	t_Cell Cells[CELL_NUMBERS][CELL_NUMBERS];

} t_Table;
