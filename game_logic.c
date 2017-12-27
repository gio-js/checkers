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

		if (piece.X == x && piece.Y == y && piece.IsTaken != 1)
			return &gameSession->Pieces[index];
	}
	return NULL;
}

/**
 * Checks for if the movement transform the piece in a king
 */
t_PieceMandatoryTake getPieceMandatoryTakes(t_GameSession *gameSession, t_Piece *currentPiece) {
	int i = 0, j = 0;
	int adiacentDiagonalCells[2][4];

	// initialize array
	for(i = 0; i < 2; i++)
		for(j = 0; j < 4; j++)
			adiacentDiagonalCells[i][j] = 0;

	// (simple piece) black only up direction
	if (currentPiece->Pawn.PawnType == PAWN_TYPE_KING || (
	            currentPiece->Pawn.PawnType == PAWN_TYPE_SIMPLE_PIECE &&
	            currentPiece->Pawn.PlayerColor == PLAYER_COLOR_WHITE
	        )) {
		adiacentDiagonalCells[0][0] = -1; // x
		adiacentDiagonalCells[0][1] = 1; // y
		adiacentDiagonalCells[0][2] = 1; // x
		adiacentDiagonalCells[0][3] = 1; // y
	}

	// (simple piece) white only down direction
	if (currentPiece->Pawn.PawnType == PAWN_TYPE_KING || (
	            currentPiece->Pawn.PawnType == PAWN_TYPE_SIMPLE_PIECE &&
	            currentPiece->Pawn.PlayerColor == PLAYER_COLOR_BLACK
	        )) {
		adiacentDiagonalCells[1][0] = -1; // x
		adiacentDiagonalCells[1][1] = -1; // y
		adiacentDiagonalCells[1][2] = 1; // x
		adiacentDiagonalCells[1][3] = -1; // y
	}

	t_PieceMandatoryTake mandatoryTake;
	mandatoryTake.Piece = NULL;
	mandatoryTake.Next = NULL;

	for(i = 0; i < 2; i++) {
		for(j = 0; j < 4; j+=2) {
			int diagonalX = adiacentDiagonalCells[i][j];
			int diagonalY = adiacentDiagonalCells[i][j+1];

			// diagonal not set
			if (diagonalX == 0 || diagonalY == 0)
				continue;

			int diagonalPieceX = diagonalX + currentPiece->X;
			int diagonalPieceY = diagonalY + currentPiece->Y;

			// check bounds
			if (diagonalPieceX < 0 || diagonalPieceY < 0)
				continue;

			if (diagonalPieceX > MAXIMUM_X_COORDINATE || diagonalPieceY > MAXIMUM_Y_COORDINATE)
				continue;

			// get diagonal piece
			t_Piece *diagonalPiece = getPieceByCoordinates(gameSession, diagonalPieceX, diagonalPieceY);

			// a piece of the opponent player has been found on this diagonal
			if (diagonalPiece != NULL &&
			        diagonalPiece->Pawn.PlayerColor != currentPiece->Pawn.PlayerColor &&
			        diagonalPiece->Pawn.PawnType <= currentPiece->Pawn.PawnType) { // check for an empty space after this piece

				int afterDiagonalPieceX = (diagonalX * 2) + currentPiece->X;
				int afterDiagonalPieceY = (diagonalY * 2) + currentPiece->Y;

				// check bounds
				if (afterDiagonalPieceX < 0 || afterDiagonalPieceY < 0)
					continue;

				if (afterDiagonalPieceX > MAXIMUM_X_COORDINATE || afterDiagonalPieceY > MAXIMUM_Y_COORDINATE)
					continue;

				t_Piece *afterDiagonalPiece = getPieceByCoordinates(gameSession, afterDiagonalPieceX, afterDiagonalPieceY);
				if (afterDiagonalPiece == NULL) { // is an empty cell
					if (mandatoryTake.Piece != NULL) {
						t_PieceMandatoryTake* lastMandatoryTake = (t_PieceMandatoryTake*)malloc(sizeof(t_PieceMandatoryTake));
						(*lastMandatoryTake) = mandatoryTake;
						mandatoryTake.Next = lastMandatoryTake;
					}

					mandatoryTake.Piece = diagonalPiece;
				}

			}
		}
	}
	return mandatoryTake;
}


/**
 * Retrieves the mandatory takes for current player
 */
t_PlayerMandatoryTake getPlayerMandatoryTakes(t_GameSession *gameSession, t_Player *player) {
	t_PlayerMandatoryTake playerTakes;
	playerTakes.ReferredPiece = NULL;
	playerTakes.MandatoryTakes.Piece = NULL;
	playerTakes.MandatoryTakes.Next = NULL;
	playerTakes.Next = NULL;

	int index = 0;
	for(index = 0; index < TABLE_PIECE_NUMBERS; index++) {
		t_Piece *refPiece = &gameSession->Pieces[index];

		if (refPiece->Player == player && refPiece->IsTaken != 1) {

			t_PieceMandatoryTake takes = getPieceMandatoryTakes(gameSession, refPiece);
			if (takes.Piece != NULL) {
				if (playerTakes.ReferredPiece != NULL) {
					t_PlayerMandatoryTake* lastPlayerTakes = (t_PlayerMandatoryTake*)malloc(sizeof(t_PlayerMandatoryTake));
					(*lastPlayerTakes) = playerTakes;
					playerTakes.Next = lastPlayerTakes;
				}

				playerTakes.ReferredPiece = refPiece;
				playerTakes.MandatoryTakes = takes;
			}

		}
	}

	return playerTakes;
}

/**
 * Checks if the specified piece is a mandatory take by specified cell piece
 */
int checkPieceIsMandatoryTake(t_PlayerMandatoryTake *mandatoryTakeList, t_Piece *piece) {
	while (mandatoryTakeList != NULL) {
		if (mandatoryTakeList->ReferredPiece != NULL) {

			t_PieceMandatoryTake* pieceTakesList = &(mandatoryTakeList->MandatoryTakes);
			while(pieceTakesList != NULL) {
				if (pieceTakesList->Piece == piece)
					return 1;

				pieceTakesList = pieceTakesList->Next;
			}

		}

		mandatoryTakeList = mandatoryTakeList->Next;
	}

	return 0;
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

	int deltaX = movement->Xto - movement->Xfrom;
	int deltaY = movement->Yto - movement->Yfrom;

	// (simple piece) white only down direction
	if (movement->Piece->Pawn.PawnType == PAWN_TYPE_SIMPLE_PIECE &&
	        movement->Piece->Pawn.PlayerColor == PLAYER_COLOR_WHITE &&
	        deltaY < 0)
		return 0;

	// (simple piece) black only up direction
	if (movement->Piece->Pawn.PawnType == PAWN_TYPE_SIMPLE_PIECE &&
	        movement->Piece->Pawn.PlayerColor == PLAYER_COLOR_BLACK &&
	        deltaY > 0)
		return 0;

	// must be a diagonal movement (single or a take)
	if (abs(deltaX) != abs(deltaY))
		return 0; // not a diagonal movement

	if (abs(deltaX) != 1 && abs(deltaX) != 2) // simple move or a take
		return 0;

	int isATake = checkIsTakeMove(movement, gameSession);
	if (abs(deltaX) == 2 && isATake == 0)
		return 0;

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
t_Piece* getTakenPieceByMove(t_Move *movement, t_GameSession *gameSession) {
	int deltaX = movement->Xto - movement->Xfrom;
	int deltaY = movement->Yto - movement->Yfrom;

	if (abs(deltaX) == 2) { // there must be a piece in the middle diagonal movement

		t_Piece *takenPiece = getPieceByCoordinates(gameSession, movement->Xfrom + (deltaX/2), movement->Yfrom + (deltaY/2));

		// filter, a simple pawn can not eat a king!
		if (takenPiece != NULL &&
		        movement->Piece->Pawn.PawnType == PAWN_TYPE_SIMPLE_PIECE &&
		        takenPiece->Pawn.PawnType == PAWN_TYPE_KING)
			return NULL;

		return takenPiece;
	}

	return NULL;
}

/**
 * Checks if the movement is a take for another piece (0:no, 1:yes)
 */
int checkIsTakeMove(t_Move *movement, t_GameSession *gameSession) {
	t_Piece *takenPiece = getTakenPieceByMove(movement, gameSession);
	return (takenPiece == NULL ? 0 : 1);
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

	// if there are any mandatory movement, only the pieces that can eat an opponent piece can move
	if (gameSession->HasPlayerMandatoryTakes == 1) {
		int isMandatoryTakePiece = 0;
		t_PlayerMandatoryTake takes = gameSession->PlayerMandatoryTakes;

		while(takes.ReferredPiece != NULL) {
			if (takes.ReferredPiece == piece) { // same piece, the movement can be done with this piece
				isMandatoryTakePiece = 1;
				break;
			}

			if (takes.Next == NULL)
				break;

			takes = *takes.Next;
		}

		if (isMandatoryTakePiece == 0)
			return 0;
	}

	return 1;
}

/**
 * Does the current user can begin a movement?
 */
int canBeginAMovement(t_GameSession *gameSession) {
	return gameSession->MovementInProgress;
}

/**
 * Is there a movement in progress?
 */
int isAnyMovementInProgress(t_GameSession *gameSession) {
	return gameSession->MovementInProgress;
}

/**
 * Undo begin movement
 */
void undoBeginPlayerMovement(t_GameSession *gameSession) {
	gameSession->MovementInProgress = 0;
	gameSession->CurrentMovements.Movement = NULL;
	gameSession->CurrentMovements.PreviousMovement = NULL;
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

	t_MoveStory currentMovements;
	currentMovements.Movement = NULL;
	currentMovements.PreviousMovement = NULL;
	currentMovements.Movement = (t_Move*)malloc(sizeof(t_Move));
	(*currentMovements.Movement) = movement;
	if (gameSession->CurrentMovements.Movement != NULL) {
		currentMovements.PreviousMovement = &gameSession->CurrentMovements;
	}
	gameSession->CurrentMovements = currentMovements;
	gameSession->MovementInProgress = 1;
}

/**
 * End player movement
 */
void endPlayerMovement(t_GameSession *gameSession, int xTo, int yTo) {

	t_Move *movement = gameSession->CurrentMovements.Movement;
	movement->Xto = xTo;
	movement->Yto = yTo;

	int check = checkIsValidMove(movement, gameSession);
	if (check == 0)
		return;

	movement->IsATake = checkIsTakeMove(movement, gameSession);
	movement->BecomeAKing = checkIsKingMove(movement, gameSession);

	int score = 0;
	score += movement->IsATake * MOVEMENT_TAKE_SCORE_WEIGHT;
	score += movement->BecomeAKing * MOVEMENT_KING_SCORE_WEIGHT;
	movement->Score = score;

	// move cell
	movement->Piece->X = xTo;
	movement->Piece->Y = yTo;

	// if take, remove cell
	if (movement->IsATake == 1) {
		t_Piece *takenPiece = getTakenPieceByMove(movement, gameSession);
		takenPiece->IsTaken = 1;
		movement->TakenPiece = takenPiece;
	}

	// if king, transform the cell in a king type
	if (movement->BecomeAKing == 1) {
		movement->Piece->Pawn.PawnType = PAWN_TYPE_KING;
	}

	// append movement to the history list
	t_MoveStory nextMoveStory;
	nextMoveStory.Movement = movement;
	nextMoveStory.PreviousMovement = gameSession->Movements;
	gameSession->Movements = &nextMoveStory;

	// switch player
	int switchPlayer = 1;

	// in case last occurred movement "is a take" and there are still mandatory take for the involved piece the player just moved,
	// then the player must not be switched ( the eat chain must continue )
	if (movement->IsATake == 1) {
		t_PlayerMandatoryTake takes = getPlayerMandatoryTakes(gameSession, gameSession->PlayerInTurn);

		while(takes.ReferredPiece != NULL) {
			if (takes.ReferredPiece == movement->Piece) {// same piece, continue the eat chain

				// can't switch player
				switchPlayer = 0;

				// begin the new take movement
				beginPlayerMovement(gameSession, movement->Xto, movement->Yto);
				break;
			}

			if (takes.Next == NULL)
				break;

			takes = *takes.Next;
		}
	}

	// switch player turn management
	if (switchPlayer == 1) {
		// clear current movents (single turn of a player)
		gameSession->MovementInProgress = 0;
		gameSession->CurrentMovements.Movement = NULL;
		gameSession->CurrentMovements.PreviousMovement = NULL;

		// switch player
		if (gameSession->PlayerInTurn == &(gameSession->FirstPlayer))
			gameSession->PlayerInTurn = &(gameSession->SecondPlayer);
		else
			gameSession->PlayerInTurn = &(gameSession->FirstPlayer);
	}

	// recalculate mandatory takes
	gameSession->PlayerMandatoryTakes = getPlayerMandatoryTakes(gameSession, gameSession->PlayerInTurn);
	gameSession->HasPlayerMandatoryTakes = 0;
	if (gameSession->PlayerMandatoryTakes.ReferredPiece != NULL)
		gameSession->HasPlayerMandatoryTakes = 1;


}
