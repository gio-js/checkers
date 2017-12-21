#include "models.h"
#include "const.h"

/**
 * Checks for a valid piece movement
 */
int checkIsValidMove(t_Move *movement, t_GameSession *gameSession) {
	return 0;	
}

/**
 * Checks for if the movement transform the piece in a king
 */
int checkIsKingMove(t_Move *movement, t_GameSession *gameSession) {
	return 0;	
}

/**
 * Checks for if the movement transform the piece in a king
 */
t_Piece* getCellByCoordinates(t_GameSession *gameSession, int x, int y) {
	int index = 0;
	for(index = 0; index < TABLE_PIECE_NUMBERS; index++) {
		t_Piece piece = gameSession->Pieces[index];
		
		if (piece.X == x && piece.Y == y)
			return &piece;
	}
	return NULL;
}
