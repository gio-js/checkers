#include "models.h"
#include "shared.h"
#include "enums.h"


/**
 * Initialize the game session
 */
void initializeGameSession() {
	SharedApplicationSession.CurrentGameSession.FirstPlayer = (t_Player) {
		"Player1", PLAYER_TYPE_HUMAN, PLAYER_COLOR_WHITE
	};

	SharedApplicationSession.CurrentGameSession.SecondPlayer = (t_Player) {
		"Computer", PLAYER_TYPE_COMPUTER, PLAYER_COLOR_BLACK
	};
	//gameSession.Movements = NULL;
	SharedApplicationSession.CurrentGameSession.movementInProgress = 0;
	SharedApplicationSession.CurrentGameSession.cursorX = 0;
	SharedApplicationSession.CurrentGameSession.cursorY = 0;
	SharedApplicationSession.CurrentGameSession.currentMove.Piece = malloc(sizeof (t_Piece*));

	// players pieces
	int index = 0;
	int cellPerRow = 4, cellXIndex = 0;
	for(index = 0; index < PLAYER_PIECE_NUMBERS; index++) {
		t_Piece p1Piece, p2Piece;
		int p1cellY = index / cellPerRow, p1cellX = 0;
		int p2cellY = MAXIMUM_X_COORDINATE - p1cellY, p2cellX = 0;
		p1cellX = (p1cellY % 2 == 0 ? cellXIndex : cellXIndex + 1 );
		p2cellX = (p1cellY % 2 == 0 ? cellXIndex + 1 : cellXIndex);

		p1Piece.Pawn = (t_Pawn) {
			PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_WHITE
		};
		p1Piece.Player = &(SharedApplicationSession.CurrentGameSession.FirstPlayer);
		p1Piece.X = p1cellX;
		p1Piece.Y = p1cellY;

		p2Piece.Pawn = (t_Pawn) {
			PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_BLACK
		};
		p2Piece.Player = &(SharedApplicationSession.CurrentGameSession.SecondPlayer);
		p2Piece.X = p2cellX;
		p2Piece.Y = p2cellY;

		if (cellXIndex+2 == (cellPerRow *2))
			cellXIndex = 0;
		else
			cellXIndex +=2;

		SharedApplicationSession.CurrentGameSession.Pieces[index] = p1Piece;
		SharedApplicationSession.CurrentGameSession.Pieces[index + PLAYER_PIECE_NUMBERS] = p2Piece;
	}

	SharedApplicationSession.CurrentGameSession.PlayerInTurn = &(SharedApplicationSession.CurrentGameSession.FirstPlayer);
}

/**
 * Generates a new local game session
 */
void initializeSharedSession() {
	initializeGameSession();
	SharedApplicationSession.ScreenUserSelection = MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME;
	SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_MENU;
}
