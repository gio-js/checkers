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
 * Defines a list of piece that must be taken in the current movement
 */
typedef struct PieceMandatoryTake {
	t_Piece *Piece; // the piece to take
	int DestinationX; // movement destination x coordinate in order to the the piece
	int DestinationY; // movement destination y coordinate in order to the the piece
	struct PieceMandatoryTake *Next;
} t_PieceMandatoryTake;

/*
 * Defines a list of piece that must be taken in the current movement
 */
typedef struct PlayerMandatoryTake {
	t_Piece *ReferredPiece; // ref piece who must start the take movement and eat the opponent piece
	t_PieceMandatoryTake MandatoryTakes; // mandatory takes elements
	struct PlayerMandatoryTake *Next;
} t_PlayerMandatoryTake;

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
	t_Piece *TakenPiece; // indicates which piece has been taken by this movement
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
 * Defines a list of available movements for the specified cell
 * NOTE: we will not store any Piece struct reference beacuse we need to stay agnostic about the current game session and memory pointers
 */
typedef struct AvailableMovement {
	int SourceX; // the piece that can be moved X coordinate
	int SourceY; // the piece that can be moved Y coordinate
	int DestinationX; // x coordinate where the specified piece can be placed
	int DestinationY; // y coordinate where the specified piece can be placed
	struct AvailableMovement* Next; // next available movement
} t_AvailableMovement;

/*
 * Defines a list of piece momement used fot IA algorithm
 */
typedef struct GameSession {
	t_Piece Pieces[TABLE_PIECE_NUMBERS]; // all the available pieces
	t_Player FirstPlayer;
	t_Player SecondPlayer;
	t_MoveStory *Movements;
	t_Player *PlayerInTurn; // the player who is in charge for the next move
	int CursorX;
	int CursorY;
	int MovementInProgress;
	t_MoveStory CurrentMovements;
	t_PlayerMandatoryTake PlayerMandatoryTakes; // the list containing the mandatory adiacent pieces the the moving pawn must take (a single one among the list)
	int HasPlayerMandatoryTakes; // a flag that indicate whether the player has any mandatory movement in order to take some opponent pieces
	t_Player *PlayerVictory; // the player who won the match
	int CPU_UnableToContinue; // The flag reaches the 1 value in case the CPU is unable to find any valid movement in order to continue the match
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
