#include "SDL2/SDL.h"
#include "const.h"
#include "models.h"
#include "shared.h"

/**
 * Render the checkers matrix (on the left side of the screen)
 */
void renderGameCheckers(SDL_Renderer* renderer, t_GameSession *gameSession, int availableWidth, int availableHeight) {

	int padding = 35;
	int squareNumbers = (MAXIMUM_X_COORDINATE + 1);
	int squareWidth = 0, squareHeight = 0;

	if (availableWidth > availableHeight) {
		squareWidth = (availableWidth - (padding * 2)) / squareNumbers;
		squareHeight = squareWidth;
	} else {
		squareHeight = (availableHeight - (padding * 2)) / squareNumbers;
		squareWidth = squareHeight;
	}

	int startX = (availableWidth - (squareWidth * squareNumbers)) / 2;
	int startY = (availableHeight - (squareHeight * squareNumbers)) / 2;
	int index = 0;



	// draw cells
	for (index = 0; index < CELL_NUMBERS; index++) {
//		if (index == SharedApplicationSession.ScreenUserSelection)
//			SDL_SetRenderDrawColor(renderer, 70, 13, 13, SDL_ALPHA_OPAQUE);

		int cellY = index / squareNumbers;
		int cellX = index - (cellY * squareNumbers);

		SDL_Rect rect;
		rect.x = startX + (cellX * squareWidth);
		rect.y = startY + (cellY * squareHeight);
		rect.w = squareWidth;
		rect.h = squareHeight;

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
		if ((cellY % 2 == 0 && cellX % 2 == 0) || (cellY % 2 != 0 && cellX % 2 != 0))
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		SDL_RenderFillRect(renderer, &rect);


		t_Piece *piece = getCellByCoordinates(gameSession, cellX, cellY);

		if (piece != NULL)
			drawPawnSimplePiece(renderer, piece->Pawn.PlayerColor, piece->Pawn.PawnType, rect.x, rect.y, rect.w, rect.h);



//		char *menuText = getMenuDescription(index);
//		int textWidth, textHeight;
//		TTF_SizeText(Sans,menuText,&textWidth,&textHeight); // get text size
//		SDL_Rect Message_rect;
//		Message_rect.x = rect.x + (width - textWidth) / 2;
//		Message_rect.y = rect.y + (height - textHeight) / 2;
//		Message_rect.w = textWidth;
//		Message_rect.h = textHeight;
//
//		drawText(menuText, COLOR_WHITE, Message_rect);

	}

	// draw table borders
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &(SDL_Rect) {
		startX,startY,squareWidth*squareNumbers,squareHeight*squareNumbers
	});

	// draw cursor rectangle
	int selectionX = startX + (gameSession->cursorX * squareWidth);
	int selectionY = startY + (gameSession->cursorY * squareHeight);
	SDL_SetRenderDrawColor(renderer, 255,165,0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &(SDL_Rect) {
		selectionX,selectionY,squareWidth,squareHeight
	});
	SDL_RenderDrawRect(renderer, &(SDL_Rect) {
		selectionX+2,selectionY+2,squareWidth-4,squareHeight-4
	});
}

/**
 * Render the entire game screen
 */
void renderGameMoveMessage(SDL_Renderer* renderer, t_GameSession *gameSession, int areaX, int areaY, int availableWidth, int availableHeight) {

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
			pieceLost += (piece.IsLost == 1 ? 1 : 0);
		else
			pieceTaken += (piece.IsLost == 1 ? 1 : 0);
	}
	pieceInGame = PLAYER_PIECE_NUMBERS - pieceLost;

	// draw player name
	char textPlayerName[100];
	strcpy(textPlayerName, "Player: ");
	strcat(textPlayerName, player->PlayerName);
	textY = areaY;
	TTF_SizeText(SansSmall,textPlayerName,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPlayerName, COLOR_WHITE, &(SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece lost
	char textPieceLost[100];
	sprintf(textPieceLost, "Pieces lost: %d", pieceLost);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceLost,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceLost, COLOR_WHITE, &(SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece lost
	char textPieceTaken[100];
	sprintf(textPieceTaken, "Pieces taken: %d", pieceTaken);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceTaken,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceLost, COLOR_WHITE, &(SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

	// draw piece in game
	char textPieceInGame[100];
	sprintf(textPieceInGame, "Pieces in game: %d", pieceInGame);
	textY += textHeight + marginTop;
	TTF_SizeText(SansSmall,textPieceInGame,&textWidth,&textHeight); // get text size
	drawText(renderer, SansSmall, textPieceInGame, COLOR_WHITE, &(SDL_Rect) {
		textX, textY, textWidth, textHeight
	});

}

/**
 * Render the entire game screen
 */
void renderGameInfo(SDL_Renderer* renderer, t_GameSession *gameSession, int areaX, int availableWidth, int availableHeight) {

	renderPlayerInfo(renderer, gameSession, &gameSession->FirstPlayer, areaX, 10, availableWidth, availableHeight);
	
	renderPlayerInfo(renderer, gameSession, &gameSession->SecondPlayer, areaX, 150, availableWidth, availableHeight);

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

