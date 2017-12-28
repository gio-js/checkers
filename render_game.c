#include "SDL2/SDL.h"
#include "const.h"
#include "models.h"
#include "shared.h"
#include "functions.h"

/**
 * Render the checkers matrix (on the left side of the screen)
 */
void renderGameCheckers(SDL_Renderer* renderer, t_GameSession *gameSession, int availableWidth, int availableHeight) {

	int padding = 35;
	int squareNumbers = (MAXIMUM_X_COORDINATE + 1);
	int squareSize = 0;
	int numbersColumnWidth = 25;
	int charactersRowHeight = 25;
	const char * horizontalChars[] = {
		"A", "B", "C", "D", "E", "F", "G", "H"
	};
	int isCurrentPlayerCPU = 0, gameEnded = 0;

	if (gameSession->PlayerInTurn->PlayerType == PLAYER_TYPE_COMPUTER) {
		isCurrentPlayerCPU = 1;
	}

	if (gameSession->PlayerVictory != NULL) {
		gameEnded = 1;
	}

	if (availableWidth > availableHeight) {
		squareSize = (availableWidth - (padding * 2) - numbersColumnWidth) / squareNumbers;
	} else {
		squareSize = (availableHeight - (padding * 2) - charactersRowHeight) / squareNumbers;
	}

	int startX = (availableWidth - (squareSize * squareNumbers) - numbersColumnWidth) / 2;
	int startY = (availableHeight - (squareSize * squareNumbers)) / 2;
	int index = 0;

	// draw horizontal characters
	for (index = 0; index <= MAXIMUM_X_COORDINATE; index++) {
		int cellY = index / squareNumbers;
		int cellX = index - (cellY * squareNumbers);

		SDL_Rect rect;
		rect.x = startX + (cellX * squareSize) + numbersColumnWidth;
		rect.y = startY;
		rect.w = squareSize;
		rect.h = numbersColumnWidth;

		// draw cell border
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &rect);

		// draw column character
		int charTextWidth = 9;
		drawText(renderer, SansSmall, horizontalChars[index], COLOR_WHITE,  (SDL_Rect) {
			rect.x+(squareSize/2)-4,startY+3,charTextWidth,20
		});
	}


	// draw cells
	for (index = 0; index < CELL_NUMBERS; index++) {
		int cellY = index / squareNumbers;
		int cellX = index - (cellY * squareNumbers);

		// get checkers piece by coordinate
		t_Piece *piece = getPieceByCoordinates(gameSession, cellX, cellY);

		SDL_Rect rect;
		rect.x = startX + numbersColumnWidth + (cellX * squareSize);
		rect.y = startY + charactersRowHeight + (cellY * squareSize);
		rect.w = squareSize;
		rect.h = squareSize;

		// draw row number
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			startX,rect.y,numbersColumnWidth,squareSize
		});

		char textRowNumber[2];
		sprintf(textRowNumber, "%d", (cellY + 1));
		drawText(renderer, SansSmall, textRowNumber, COLOR_WHITE,  (SDL_Rect) {
			startX+8,rect.y+(squareSize/2)-10,9,20
		});

		// draw cell border
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		if ((cellY % 2 == 0 && cellX % 2 == 0) || (cellY % 2 != 0 && cellX % 2 != 0))
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		// mandatory take?
		if (gameSession->HasPlayerMandatoryTakes == 1) {
			int mandatoryTake = checkPieceIsMandatoryTake(&gameSession->PlayerMandatoryTakes, piece);
			if (mandatoryTake == 1)
				SDL_SetRenderDrawColor(renderer, 76, 153, 0, SDL_ALPHA_OPAQUE); // light green
		}

		SDL_RenderFillRect(renderer, &rect);

		// draw piece
		if (piece != NULL)
			drawPiece(renderer, piece->Pawn.PlayerColor, piece->Pawn.PawnType, rect.x, rect.y, rect.w, rect.h);
	}

	// draw table borders
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &(SDL_Rect) {
		startX+numbersColumnWidth,startY+charactersRowHeight,squareSize*squareNumbers,squareSize*squareNumbers
	});

	// draw cursor rectangle
	if (isCurrentPlayerCPU == 0 && gameEnded == 0) {
		int selectionX = startX + (gameSession->CursorX * squareSize) + numbersColumnWidth;
		int selectionY = startY + (gameSession->CursorY * squareSize) + charactersRowHeight;

		if (isAnyMovementInProgress(gameSession)==0)
			SDL_SetRenderDrawColor(renderer, 255,165,0, SDL_ALPHA_OPAQUE); // orange
		else
			SDL_SetRenderDrawColor(renderer, 236,39,243, SDL_ALPHA_OPAQUE); // violet

		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			selectionX,selectionY,squareSize,squareSize
		});
		SDL_RenderDrawRect(renderer, &(SDL_Rect) {
			selectionX+2,selectionY+2,squareSize-4,squareSize-4
		});
	}

	// draw player victory rectangle
	if (gameEnded == 1) {
		char textWinnerPlayer[100];
		int winnerRectWidthPerc = 60;
		int winnerRectHeightPerc = 40;
		int winnerRectWidth = availableWidth * ((float)winnerRectWidthPerc / 100.0);
		int winnerRectHeight = availableHeight * ((float)winnerRectHeightPerc / 100.0);
		int winnerRectX = (availableWidth - winnerRectWidth)/2;
		int winnerRectY = (availableHeight - winnerRectHeight)/2;
		int textWinnerWidth = 0, textWinnerHeight = 0;

		SDL_SetRenderDrawColor(renderer, 236,39,243, SDL_ALPHA_OPAQUE); // violet
		SDL_RenderFillRect(renderer, &(SDL_Rect) {
			winnerRectX, winnerRectY,winnerRectWidth,winnerRectHeight
		});

		sprintf(textWinnerPlayer, "%s won the match", gameSession->PlayerVictory->PlayerName);
		TTF_SizeText(SansLarge,textWinnerPlayer,&textWinnerWidth,&textWinnerHeight); // get text size
		drawText(renderer, SansSmall, textWinnerPlayer, COLOR_WHITE, (SDL_Rect) {
			winnerRectX + ((winnerRectWidth - textWinnerWidth) / 2),
			            winnerRectY + ((winnerRectHeight - textWinnerHeight) / 2),
			            textWinnerWidth, textWinnerHeight
		});
	}
}

/**
 * Render the entire game screen
 */
void renderGameMoveMessage(SDL_Renderer* renderer, t_GameSession *gameSession, int areaX, int areaY, int availableWidth, int availableHeight) {
	int paddingLeft = 10;
	availableWidth = availableWidth - paddingLeft;

	int textX = areaX + paddingLeft, textY = areaY, textWidth = 0, textHeight = 0, marginTop = 10;
	SDL_SetRenderDrawColor(renderer, 67,202,18, SDL_ALPHA_OPAQUE);

	// draw player in turn (in charge for the next move)
	char textMoveInfo[100];
	sprintf(textMoveInfo, "Player in turn: %s", gameSession->PlayerInTurn->PlayerName);

	TTF_SizeText(SansSmall,textMoveInfo,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textMoveInfo, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw info command
	if (isAnyMovementInProgress(gameSession)==0)
		sprintf(textMoveInfo, "Press Enter to select the piece to move. ESC to leave the game.");
	else if (isAnyMovementInProgress(gameSession)==1)
		sprintf(textMoveInfo, "Press Enter to release the piece or ESC to change selection.");

	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall, textMoveInfo, &textWidth, &textHeight); // get text size

	textHeight = 5 + textHeight + (textHeight * (textWidth / availableWidth));
	drawWrappedText(renderer, SansSmall, textMoveInfo, COLOR_WHITE, (SDL_Rect) {
		textX, textY, availableWidth, textHeight
	}, availableWidth);

	// draw some legend info
	sprintf(textMoveInfo, "Orange rect = no piece selected");
	textY += textHeight + marginTop * 2;
	TTF_SizeText(SansSmall,textMoveInfo,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textMoveInfo, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	sprintf(textMoveInfo, "Violet rect = move in progress");
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textMoveInfo,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textMoveInfo, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	sprintf(textMoveInfo, "Green cell = mandatory take");
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textMoveInfo,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textMoveInfo, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});
}

/**
 * Render player informations
 */
void renderPlayerInfo(SDL_Renderer* renderer, t_GameSession *gameSession, t_Player *player, int areaX, int areaY, int availableWidth, int availableHeight) {
	int leftPadding = 10;
	int marginTop = 5;
	int textWidth, textHeight, textX = areaX + leftPadding, textY;
	int pieceTaken = 0, pieceLost = 0, pieceInGame = 0;

	int indexPiece = 0;
	for (indexPiece = 0; indexPiece < TABLE_PIECE_NUMBERS; indexPiece++) {
		t_Piece piece = gameSession->Pieces[indexPiece];
		if (piece.Player == player)
			pieceLost += (piece.IsTaken == 1 ? 1 : 0);
		else
			pieceTaken += (piece.IsTaken == 1 ? 1 : 0);
	}
	pieceInGame = PLAYER_PIECE_NUMBERS - pieceLost;

	// draw player name
	char textPlayerName[100];
	sprintf(textPlayerName, "Player: %s (%s)", player->PlayerName, player->PlayerColor == PLAYER_COLOR_BLACK ? "BLACK" : "WHITE");
	textY = areaY;
	TTF_SizeText(SansSmall,textPlayerName,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPlayerName, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece lost
	char textPieceLost[100];
	sprintf(textPieceLost, "Pieces lost: %d", pieceLost);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceLost,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceLost, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece lost
	char textPieceTaken[100];
	sprintf(textPieceTaken, "Pieces taken: %d", pieceTaken);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceTaken,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceTaken, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece in game
	char textPieceInGame[100];
	sprintf(textPieceInGame, "Pieces in game: %d", pieceInGame);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceInGame,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceInGame, COLOR_WHITE, (SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

}

/**
 * Render the entire game screen
 */
void renderGameInfo(SDL_Renderer* renderer, t_GameSession *gameSession, int areaX, int availableWidth, int availableHeight) {

	renderPlayerInfo(renderer, gameSession, &gameSession->FirstPlayer, areaX, 10, availableWidth, availableHeight);

	renderPlayerInfo(renderer, gameSession, &gameSession->SecondPlayer, areaX, 150, availableWidth, availableHeight);

	renderGameMoveMessage(renderer, gameSession, areaX, 290, availableWidth, availableHeight);
}

/**
 * Render the entire game screen
 */
void renderGame(SDL_Renderer* renderer, t_GameSession *gameSession) {
	int checkersGameWidth = SCREEN_WIDTH * 0.7;
	int gameInfoWidth = SCREEN_WIDTH - checkersGameWidth;

	// render the checkers table
	renderGameCheckers(renderer, gameSession, checkersGameWidth, SCREEN_HEIGHT);

	// render the current game informations
	renderGameInfo(renderer, gameSession, checkersGameWidth, gameInfoWidth, SCREEN_HEIGHT);
}

