#include "models.h"
#include "shared.h"
#include "enums.h"


/**
 * Initialize the game session
 */
void initializeGameSession(int gamePresetsType) {
	SharedApplicationSession.CurrentGameSession.FirstPlayer = (t_Player) {
		"Player1", PLAYER_TYPE_HUMAN, PLAYER_COLOR_WHITE
	};

	SharedApplicationSession.CurrentGameSession.SecondPlayer = (t_Player) {
		"Computer", PLAYER_TYPE_COMPUTER, PLAYER_COLOR_BLACK
	};
	//gameSession.Movements = NULL;
	SharedApplicationSession.CurrentGameSession.MovementInProgress = 0;
	SharedApplicationSession.CurrentGameSession.CursorX = 0;
	SharedApplicationSession.CurrentGameSession.CursorY = 0;

	SharedApplicationSession.CurrentGameSession.PlayerMandatoryTakes.ReferredPiece = NULL;
	SharedApplicationSession.CurrentGameSession.PlayerMandatoryTakes.MandatoryTakes.Piece = NULL;
	SharedApplicationSession.CurrentGameSession.PlayerMandatoryTakes.MandatoryTakes.Next = NULL;
	SharedApplicationSession.CurrentGameSession.PlayerMandatoryTakes.Next = NULL;
	SharedApplicationSession.CurrentGameSession.HasPlayerMandatoryTakes = 0;
	SharedApplicationSession.CurrentGameSession.PlayerInTurn = &(SharedApplicationSession.CurrentGameSession.FirstPlayer);

	// players pieces (default preset initial game)
	if (gamePresetsType == GAME_PRESETS_TYPE_DEFAULT) {
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
			p1Piece.IsTaken = 0;
			p2Piece.Pawn = (t_Pawn) {
				PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_BLACK
			};
			p2Piece.Player = &(SharedApplicationSession.CurrentGameSession.SecondPlayer);
			p2Piece.X = p2cellX;
			p2Piece.Y = p2cellY;
			p2Piece.IsTaken = 0;

			if (cellXIndex+2 == (cellPerRow *2))
				cellXIndex = 0;
			else
				cellXIndex +=2;

			SharedApplicationSession.CurrentGameSession.Pieces[index] = p1Piece;
			SharedApplicationSession.CurrentGameSession.Pieces[index + PLAYER_PIECE_NUMBERS] = p2Piece;
		}
	} else if (gamePresetsType == GAME_PRESETS_TYPE_TEST1) {
		t_Piece p1Piece = (t_Piece) {
			(t_Pawn) {
				PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_WHITE
			},
			&SharedApplicationSession.CurrentGameSession.FirstPlayer,
			2,
			2,
			0
		};

		t_Piece p2Piece = (t_Piece) {
			(t_Pawn) {
				PAWN_TYPE_KING, PLAYER_COLOR_BLACK
			},
			&SharedApplicationSession.CurrentGameSession.SecondPlayer,
			0,
			0,
			0
		};

		t_Piece p3Piece = (t_Piece) {
			(t_Pawn) {
				PAWN_TYPE_SIMPLE_PIECE, PLAYER_COLOR_WHITE
			},
			&SharedApplicationSession.CurrentGameSession.FirstPlayer,
			4,
			2,
			0
		};
		SharedApplicationSession.CurrentGameSession.Pieces[0] = p1Piece;
		SharedApplicationSession.CurrentGameSession.Pieces[1] = p2Piece;
		SharedApplicationSession.CurrentGameSession.Pieces[2] = p3Piece;
	}

}

/**
 * Generates a new local game session
 */
void initializeSharedSession() {
	SharedApplicationSession.ScreenUserSelection = MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME;
	SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_MENU;
}
