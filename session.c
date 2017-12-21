#include "models.h"
#include "shared.h"
#include "enums.h"

/**
 * Generates a new local game session
 */
void initializeSharedSession() {
	t_GameSession gameSession;

	gameSession.FirstPlayer = (t_Player) {
		"Player1", PLAYER_TYPE_HUMAN, PLAYER_COLOR_WHITE
	};
	
	gameSession.SecondPlayer = (t_Player) {
		"Computer", PLAYER_TYPE_COMPUTER, PLAYER_COLOR_BLACK
	};
	
	gameSession.PlayerInTurn = &gameSession.FirstPlayer;
	gameSession.Movements = NULL;
	gameSession.cursorX = 0;
	gameSession.cursorY = 0;

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
		p1Piece.Player = &gameSession.FirstPlayer;
		p1Piece.X = p1cellX;
		p1Piece.Y = p1cellY;

		p2Piece.Pawn = (t_Pawn) {
			PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_BLACK
		};
		p2Piece.Player = &gameSession.SecondPlayer;
		p2Piece.X = p2cellX;
		p2Piece.Y = p2cellY;

		if (cellXIndex+2 == (cellPerRow *2))
			cellXIndex = 0;
		else
			cellXIndex +=2;

		gameSession.Pieces[index] = p1Piece;
		gameSession.Pieces[index + PLAYER_PIECE_NUMBERS] = p2Piece;
	}

	SharedApplicationSession.CurrentGameSession = gameSession;
	SharedApplicationSession.ScreenUserSelection = MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME;
	SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_MENU;
}
