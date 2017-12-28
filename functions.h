#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "models.h"
#include "const.h"
#include "enums.h"

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// session
void initializeGameSession(int gamePresetsType);
void initializeSharedSession();

// game_logic
t_Piece* getPieceByCoordinates(t_GameSession *gameSession, int x, int y);
int** getValidPieceDiagonalOffset(t_Piece *piece);
t_PieceMandatoryTake getPieceMandatoryTakes(t_GameSession *gameSession, t_Piece *currentPiece);
t_PlayerMandatoryTake getPlayerMandatoryTakes(t_GameSession *gameSession, t_Player *player);
int checkPieceIsMandatoryTake(t_PlayerMandatoryTake *mandatoryTakeList, t_Piece *piece);
int getCellTypeColor(int x, int y);
int checkIsValidMove(t_Move *movement, t_GameSession *gameSession);
int checkIsKingMove(t_Move *movement, t_GameSession *gameSession);
t_Piece* getTakenPieceByMove(t_Move *movement, t_GameSession *gameSession);
int checkIsTakeMove(t_Move *movement, t_GameSession *gameSession);
int checkForPlayerMovement(t_GameSession *gameSession, int x, int y);
int canBeginAMovement(t_GameSession *gameSession);
int isAnyMovementInProgress(t_GameSession *gameSession);
void undoBeginPlayerMovement(t_GameSession *gameSession);
void beginPlayerMovement(t_GameSession *gameSession, int x, int y);
int endPlayerMovement(t_GameSession *gameSession, int xTo, int yTo);
t_AvailableMovement getPieceAvailableMovements(t_GameSession *gameSession, t_Piece *piece);
t_AvailableMovement getAvailableMovements(t_GameSession *gameSession, t_Player *player);
t_GameSession *cloneGameSession(t_GameSession *gameSession);

// ai_strategy
int analyzeMovementPath(t_GameSession *gameSession, t_AvailableMovement movement, int level);
t_AvailableMovement findBestAvailableMovement(t_GameSession *gameSession);
int makeMovementByCPU(t_GameSession *gameSession);

// render_generic
void drawText(SDL_Renderer* renderer, TTF_Font *font, char *text, SDL_Color color, SDL_Rect rect);
void drawWrappedText(SDL_Renderer* renderer, TTF_Font *font, char *text, SDL_Color color, SDL_Rect rect, int maxWidth);
void drawPiece(SDL_Renderer* renderer, int playerColor, int pawnType, int x, int y, int width, int height);

// render_manager
void handleMenuEvents(SDL_KeyboardEvent key);
void handleGameEvents(SDL_KeyboardEvent key);
void handleEvents(SDL_Event event);
void renderMenu();
void renderScene();

// render_game
void renderGameCheckers(SDL_Renderer* renderer, t_GameSession *gameSession, int availableWidth, int availableHeight);
void renderGameMoveMessage(SDL_Renderer* renderer, t_GameSession *gameSession,
                           int areaX, int areaY, int availableWidth, int availableHeight);
void renderPlayerInfo(SDL_Renderer* renderer, t_GameSession *gameSession, t_Player *player,
                      int areaX, int areaY, int availableWidth, int availableHeight);
void renderGameInfo(SDL_Renderer* renderer, t_GameSession *gameSession, int areaX, int availableWidth, int availableHeight);
void renderGame(SDL_Renderer* renderer, t_GameSession *gameSession);

#endif
