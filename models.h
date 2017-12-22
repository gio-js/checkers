#include "enums.h"
#include "const.h"

#ifndef MODELS_H
#define MODELS_H

/**
 * Defines the pawn structure
 */
typedef struct Pawn {
	enum PAWN_TYPE PawnType; // simple
	enum PLAYER_COLOR PlayerColor;
} t_Pawn;

/*
 * Defines the structure data of the player
 */
typedef struct Player {
	char* PlayerName; // player name
	enum PLAYER_TYPE PlayerType;
	enum PLAYER_COLOR PlayerColor;
} t_Player;

/*
 * Defines a checkers piece structure
 */
typedef struct Piece {
	t_Pawn Pawn; // null if this is case of empty cell, otherwise pointing to a valid mem element
	t_Player *Player; // piece related player
	int X; // from left to right, starting from 0 to 15
	int Y; // from top to bottom, starting from 0 to 15
	int IsTaken; // piece taken by the opponent
} t_Piece;

/*
 * Defines the structure of a piece movement
 */
typedef struct Move {
	t_Piece *Piece; // the piece the player is moving
	int Xfrom; // souce movement position
	int Yfrom;
	int Xto; // destination movement position
	int Yto;
	int IsATake; // indicates if the movement ends as a take for the player
	int BecomeAKing; // indicates if the movment transform the piece to a king type
	double Score; // define a virtual score for the movevement (i.e. if the movement ends with a take, the score is increased as 1)
} t_Move;

/*
 * Defines a list of piece momement used fot IA algorithm
 */
typedef struct MoveStory {
	t_Move *Movement;
	struct MoveStory *PreviousMovement;
} t_MoveStory;

/*
 * Defines a list of piece momement used fot IA algorithm
 */
typedef struct GameSession {
	t_Piece Pieces[TABLE_PIECE_NUMBERS]; // all the available players
	t_Player FirstPlayer;
	t_Player SecondPlayer;
	t_MoveStory *Movements;
	t_Player *PlayerInTurn; // the player who is in charge for the next move
	int cursorX;
	int cursorY;
	int movementInProgress;
	t_Move currentMove;
} t_GameSession;



/*
 * Utility structure, contains the main application context session datas
 */
typedef struct ApplicationSession {

	/**
	 * Contains the current application game session
	 */
	t_GameSession CurrentGameSession;

	/**
	 * Menu user selection
	 */
	enum MENU_ELEMENT_TYPE ScreenUserSelection;
	
	/**
	 * Current game screen
	 */
	enum APPLICATION_SCREENS CurrentScreen;


} t_ApplicationSession;


#endif
