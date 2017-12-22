#include "models.h"
#include "const.h"
#include "enums.h"

/**
 * Checks for if the movement transform the piece in a king
 */
t_Piece* getPieceByCoordinates(t_GameSession *gameSession, int x, int y) {
	int index = 0;
	for(index = 0; index < TABLE_PIECE_NUMBERS; index++) {
		t_Piece piece = gameSession->Pieces[index];

		if (piece.X == x && piece.Y == y)
			return &gameSession->Pieces[index];
	}
	return NULL;
}

/**
 * Retrieves the specified cell color
 */
int getCellTypeColor(int x, int y) {
	if (y % 2 == 0)
		return (x % 2 == 0 ? CELL_TYPE_WHITE : CELL_TYPE_BLACK);
	else
		return (x % 2 == 0 ? CELL_TYPE_BLACK : CELL_TYPE_WHITE);
}

/**
 * Checks for a valid piece movement (0:no, 1:yes)
 */
int checkIsValidMove(t_Move *movement, t_GameSession *gameSession) {
	// inside table bounds
	if (movement->Xto > MAXIMUM_X_COORDINATE || movement->Yto > MAXIMUM_Y_COORDINATE ||
	        movement->Xto < 0 || movement->Yto < 0)
		return 0;

	// correct destination cell color
	int cellType = getCellTypeColor(movement->Xto, movement->Yto);
	if (cellType != CELL_TYPE_WHITE)
		return 0;

	// destination must be an empty cell
	t_Piece *destination = getPieceByCoordinates(gameSession, movement->Xto, movement->Yto);
	if (destination != NULL)
		return 0;

	// must be a diagonal movement (single or a take)
	int deltaX = movement->Xto - movement->Xfrom;
	int deltaY = movement->Yto - movement->Yfrom;
	if (deltaX != deltaY)
		return 0; // not a diagonal movement

	if (abs(deltaX) != 1 && abs(deltaX) != 3) // simple move or a take
		return 0;

	if (abs(deltaX) == 3) {// there must be a piece in the middle diagonal movement
		int gap = (deltaX > 0 ? 1: -1);
		t_Piece *takenPiece = getPieceByCoordinates(gameSession, movement->Xfrom + gap, movement->Yto + gap);

		if (takenPiece == NULL)
			return 0;
	}

	return 1;
}

/**
 * Checks if the movement transform the piece in a king (0:no, 1:yes)
 */
int checkIsKingMove(t_Move *movement, t_GameSession *gameSession) {
	switch(gameSession->PlayerInTurn->PlayerColor) {
		case PLAYER_COLOR_BLACK:
			return (movement->Yto == 0 ? 1 : 0);

		case PLAYER_COLOR_WHITE:
			return (movement->Yto == MAXIMUM_Y_COORDINATE ? 1 : 0);
	}
}

/**
 * Checks if the movement is a take for another piece (0:no, 1:yes)
 */
int checkIsTakeMove(t_Move *movement, t_GameSession *gameSession) {
	int deltaX = movement->Xto - movement->Xfrom;
	int deltaY = movement->Yto - movement->Yfrom;

	if (abs(deltaX) == 3) {// there must be a piece in the middle diagonal movement
		int gap = (deltaX > 0 ? 1: -1);
		t_Piece *takenPiece = getPieceByCoordinates(gameSession, movement->Xfrom + gap, movement->Yto + gap);

		if (takenPiece == NULL)
			return 1;
	}

	return 0;
}

/**
 * Checks if the movement is a take for another piece (0:no, 1:yes)
 */
t_Piece* getTakenPieceByMove(t_Move *movement, t_GameSession *gameSession) {
	int deltaX = movement->Xto - movement->Xfrom;
	int deltaY = movement->Yto - movement->Yfrom;

	if (abs(deltaX) == 3) {// there must be a piece in the middle diagonal movement
		int gap = (deltaX > 0 ? 1: -1);
		return getPieceByCoordinates(gameSession, movement->Xfrom + gap, movement->Yto + gap);
	}

	return NULL;
}

/**
 * Begin player movement
 */
int checkForPlayerMovement(t_GameSession *gameSession, int x, int y) {
	t_Piece *piece = getPieceByCoordinates(gameSession, x, y);

	if (piece == NULL)
		return 0;

	if (piece->Player != gameSession->PlayerInTurn)
		return 0;

	return 1;
}

/**
 * Does the current user can begin a movement?
 */
int canBeginAMovement(t_GameSession *gameSession) {
	return gameSession->movementInProgress;
}

/**
 * Is there a movement in progress?
 */
int isAnyMovementInProgress(t_GameSession *gameSession) {
	return gameSession->movementInProgress;
}

/**
 * Undo begin movement
 */
void undoBeginPlayerMovement(t_GameSession *gameSession) {
	gameSession->movementInProgress = 0;
}

/**
 * Begin player movement
 */
void beginPlayerMovement(t_GameSession *gameSession, int x, int y) {
	int check = checkForPlayerMovement(gameSession, x, y);
	if (check == 0)
		return;

	t_Move movement;
	movement.Piece = getPieceByCoordinates(gameSession, x, y);
	movement.Xfrom = x;
	movement.Yfrom = y;
	movement.Xto = -1;
	movement.Yto = -1;
	movement.IsATake = 0;
	movement.BecomeAKing = 0;
	movement.Score = 0;

	//gameSession->currentMove = malloc(sizeof(*t_Move));
	gameSession->currentMove = movement;
	gameSession->movementInProgress = 1;
}

/**
 * End player movement
 */
void endPlayerMovement(t_GameSession *gameSession, int xTo, int yTo) {

	t_Move movement = gameSession->currentMove;
	movement.Xto = xTo;
	movement.Yto = yTo;

	int check = checkIsValidMove(&movement, gameSession);
	if (check == 0)
		return;

	movement.IsATake = checkIsTakeMove(&movement, gameSession);
	movement.BecomeAKing = checkIsKingMove(&movement, gameSession);

	int score = 0;
	score += movement.IsATake * MOVEMENT_TAKE_SCORE_WEIGHT;
	score += movement.BecomeAKing * MOVEMENT_KING_SCORE_WEIGHT;
	movement.Score = score;

	// move cell
	movement.Piece->X = xTo;
	movement.Piece->Y = yTo;

	// if take, remove cell
	if (movement.IsATake == 1) {
		t_Piece *takenPiece = getTakenPieceByMove(&movement, gameSession);
		takenPiece->IsTaken = 1;
	}

	// if king, transform the cell in a king type
	if (movement.BecomeAKing == 1) {
		movement.Piece->Pawn.PawnType = PAWN_TYPE_KING;
	}

	// append movement to the list
	t_MoveStory nextMoveStory;
	nextMoveStory.Movement = &movement;
	nextMoveStory.PreviousMovement = gameSession->Movements;
	gameSession->Movements = &nextMoveStory;

	// clear current move
	gameSession->movementInProgress = 0;

	// switch player
	if (gameSession->PlayerInTurn == &gameSession->FirstPlayer)
		gameSession->PlayerInTurn = &gameSession->SecondPlayer;
	else
		gameSession->PlayerInTurn = &gameSession->FirstPlayer;

}
