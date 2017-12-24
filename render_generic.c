#include "SDL2/SDL_ttf.h"
#include "enums.h"
#include "const.h"

static SDL_Texture *pawnSimplePieceTextureWhite = NULL;
static SDL_Texture *pawnKingTextureWhite = NULL;
static SDL_Texture *pawnSimplePieceTextureBlack = NULL;
static SDL_Texture *pawnKingTextureBlack = NULL;

/**
 * Draws specified text over the SDL renderer
 */
void drawText(SDL_Renderer* renderer, TTF_Font *font, char *text, SDL_Color color, SDL_Rect rect) {
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_RenderCopy(renderer, Message, NULL, &rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

/**
 * Draws specified text over the SDL renderer
 */
void drawWrappedText(SDL_Renderer* renderer, TTF_Font *font, char *text, SDL_Color color, SDL_Rect rect, int maxWidth) {
	SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, text, color, maxWidth);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_RenderCopy(renderer, Message, NULL, &rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

/**
 * Render cell piece image
 */
void drawPiece(SDL_Renderer* renderer, int playerColor, int pawnType, int x, int y, int width, int height) {
	SDL_Texture* pawnSurface;
	SDL_Surface* image = NULL;

	switch(playerColor) {
		case PLAYER_COLOR_WHITE:

			if (pawnType == PAWN_TYPE_SIMPLE_PIECE) {
				if (pawnSimplePieceTextureWhite == NULL) {
					image = (SDL_Surface*)IMG_Load("images/pawn_simple_piece_white.png");
					pawnSimplePieceTextureWhite = SDL_CreateTextureFromSurface(renderer, image);
				}
				pawnSurface = pawnSimplePieceTextureWhite;
			} else {
				if (pawnKingTextureWhite == NULL) {
					image = (SDL_Surface*)IMG_Load("images/pawn_king_white.png");
					pawnKingTextureWhite = SDL_CreateTextureFromSurface(renderer, image);
				}
				pawnSurface = pawnKingTextureWhite;
			}

			break;
		case PLAYER_COLOR_BLACK:
			if (pawnType == PAWN_TYPE_SIMPLE_PIECE) {
				if (pawnSimplePieceTextureBlack == NULL) {
					image = (SDL_Surface*)IMG_Load("images/pawn_simple_piece_black.png");
					pawnSimplePieceTextureBlack = SDL_CreateTextureFromSurface(renderer, image);
				}
				pawnSurface = pawnSimplePieceTextureBlack;
			} else {
				if (pawnKingTextureBlack == NULL) {
					image = (SDL_Surface*)IMG_Load("images/pawn_king_black.png");
					pawnKingTextureBlack = SDL_CreateTextureFromSurface(renderer, image);
				}
				pawnSurface = pawnKingTextureBlack;
			}
			break;
	}

	if (image != NULL)
		SDL_FreeSurface(image);

	SDL_RenderCopy(renderer, pawnSurface, &(SDL_Rect){0,0,200,200}, &(SDL_Rect){x, y, width, height});
}
