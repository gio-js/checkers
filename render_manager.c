#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "enums.h"
#include "const.h"
#include "shared.h"
#include "functions.h"

// create renderer
static SDL_Renderer* renderer = NULL;

/**
 * handle event for the menu screen
 */
void handleMenuEvents(SDL_KeyboardEvent key) {
	switch(key.keysym.sym) {
		case SDLK_DOWN:
			SharedApplicationSession.ScreenUserSelection ++;
			if (SharedApplicationSession.ScreenUserSelection >= MENU_COUNT) {
				SharedApplicationSession.ScreenUserSelection = 0;
			}
			break;
		case SDLK_UP:
			if (SharedApplicationSession.ScreenUserSelection == 0) {
				SharedApplicationSession.ScreenUserSelection = MENU_COUNT - 1;
			} else {
				SharedApplicationSession.ScreenUserSelection --;
			}
			break;
		case SDLK_RETURN:
			switch(SharedApplicationSession.ScreenUserSelection) {
				case MENU_ELEMENT_TYPE_1P_VS_2P:
				case MENU_ELEMENT_TYPE_1P_VS_CPU:
					SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_GAME;

					int presetType = GAME_PRESETS_TYPE_1P_VS_2P;
					if (SharedApplicationSession.ScreenUserSelection == MENU_ELEMENT_TYPE_1P_VS_CPU)
						presetType = GAME_PRESETS_TYPE_1P_VS_CPU;

					// re-initialize game session
					initializeGameSession(presetType);
					break;
				case MENU_ELEMENT_TYPE_EXIT:
					SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_EXIT;
					break;
			}
			break;
	}
}

/**
 * handle event for the in game screen
 */
void handleGameEvents(SDL_KeyboardEvent key) {
	t_GameSession *gameSession = &SharedApplicationSession.CurrentGameSession;
	int gameEnded = 0;

	if (gameSession->PlayerVictory != NULL) {
		gameEnded = 1;
	}

	// handle key event
	switch(key.keysym.sym) {

		case SDLK_DOWN:
			if (gameEnded == 0) {
				gameSession->CursorY ++;
				if (gameSession->CursorY > MAXIMUM_Y_COORDINATE) {
					gameSession->CursorY = 0;
				}
			}
			break;

		case SDLK_UP:
			if (gameEnded == 0) {
				if (gameSession->CursorY == 0) {
					gameSession->CursorY = MAXIMUM_Y_COORDINATE;
				} else {
					gameSession->CursorY --;
				}
			}
			break;

		case SDLK_RIGHT:
			if (gameEnded == 0) {
				gameSession->CursorX ++;
				if (gameSession->CursorX > MAXIMUM_X_COORDINATE) {
					gameSession->CursorX = 0;
				}
			}
			break;

		case SDLK_LEFT:
			if (gameEnded == 0) {
				if (gameSession->CursorX == 0) {
					gameSession->CursorX = MAXIMUM_X_COORDINATE;
				} else {
					gameSession->CursorX --;
				}
			}
			break;
		case SDLK_ESCAPE:
			if (isAnyMovementInProgress(gameSession) == 1) {
				undoBeginPlayerMovement(gameSession);
			} else {
				SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_MENU;
			}
			break;
		case SDLK_RETURN:
			if (isAnyMovementInProgress(gameSession) == 1) {
				endPlayerMovement(gameSession, gameSession->CursorX, gameSession->CursorY);
			} else {
				beginPlayerMovement(gameSession, gameSession->CursorX, gameSession->CursorY);
			}
			break;
	}
}

/**
 * Handle the game events inside the drawing strong loop
 */
void handleEvents(SDL_Event event) {
	// handle an exit interrupt
	while (SDL_PollEvent(&event)) {

		// handle quit
		if (event.type == SDL_QUIT) {
			SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_EXIT;
			break;
		}

		// handle only keyup
		if (event.type != SDL_KEYUP)
			continue;

		// handle events for each screen
		switch(SharedApplicationSession.CurrentScreen) {

			case APPLICATION_SCREENS_MENU:
				handleMenuEvents(event.key);
				break;

			case APPLICATION_SCREENS_GAME:
				handleGameEvents(event.key);
				break;

		}

	}
}

/**
 * Render main game menu
 */
void renderMenu() {

	int index = 0;
	int width = 250, height = 60, marginTop = 20;
	int startY = (SCREEN_HEIGHT - (height * MENU_COUNT) - (marginTop * (MENU_COUNT - 1))) / 2;

	for (index = 0; index < MENU_COUNT; index++) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

		if (index == SharedApplicationSession.ScreenUserSelection)
			SDL_SetRenderDrawColor(renderer, 70, 13, 13, SDL_ALPHA_OPAQUE);

		SDL_Rect rect;
		rect.x = (SCREEN_WIDTH - width) / 2;
		rect.y = startY + (marginTop * index) + (height * index);
		rect.w = width;
		rect.h = height;
		SDL_RenderDrawRect(renderer, &rect);

		char *menuText = getMenuDescription(index);
		int textWidth, textHeight;
		TTF_SizeText(SansMedium,menuText,&textWidth,&textHeight); // get text size
		SDL_Rect Message_rect;
		Message_rect.x = rect.x + (width - textWidth) / 2;
		Message_rect.y = rect.y + (height - textHeight) / 2;
		Message_rect.w = textWidth;
		Message_rect.h = textHeight;


		drawText(renderer, SansMedium, menuText, COLOR_WHITE, Message_rect);
	}

}

/**
 * Handle CPU player
 */
void handleComputerPlayer() {
	t_GameSession *gameSession = &SharedApplicationSession.CurrentGameSession;
	
	if (gameSession->PlayerVictory == NULL && // game still active
		gameSession->PlayerInTurn != NULL && // player in turn is valirized
		gameSession->PlayerInTurn->PlayerType == PLAYER_TYPE_COMPUTER) { // the player in turn is the computer

		do {
			int result = makeMovementByCPU(gameSession);

			if (result == 0) {
				gameSession->CPU_UnableToContinue = 1;
				gameSession->PlayerVictory = (gameSession->PlayerInTurn == &gameSession->FirstPlayer ?
				                              &gameSession->SecondPlayer : &gameSession->FirstPlayer);
				break;
			}
		} while(gameSession->MovementInProgress == 1);

	}
}


/**
 * Renders the entire game scene, based on the application session status
 */
void renderScene() {

	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() != -1 && IMG_Init(IMG_INIT_PNG) != -1) {

		// create shared font
		SansSmall = TTF_OpenFont("OpenSans-Light.ttf", 14);
		SansMedium = TTF_OpenFont("OpenSans-Light.ttf", 20);
		SansLarge = TTF_OpenFont("OpenSans-Light.ttf", 30);

		// create window
		SDL_Window* window = NULL;

		if (SDL_CreateWindowAndRenderer((int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, 0, &window, &renderer) == 0) {

			// begin the drawing strong loop
			while (SharedApplicationSession.CurrentScreen != APPLICATION_SCREENS_EXIT) {

				// strong loop event
				SDL_Event event;

				// clear screen
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
				SDL_RenderClear(renderer);

				// handle CPU strategy
				handleComputerPlayer();

				// render game screen
				switch(SharedApplicationSession.CurrentScreen) {

					case APPLICATION_SCREENS_MENU:
						renderMenu();
						break;

					case APPLICATION_SCREENS_GAME:
						renderGame(renderer, &SharedApplicationSession.CurrentGameSession);
						break;

				}

				// blit on screen renderer changes
				SDL_RenderPresent(renderer);

				// handle events
				handleEvents(event);
			}
		}

		// destroy renderer
		if (renderer) {
			SDL_DestroyRenderer(renderer);
		}

		// destroy window
		if (window) {
			SDL_DestroyWindow(window);
		}
	}

	IMG_Quit();
	SDL_Quit();
}
