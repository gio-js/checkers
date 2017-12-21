#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "enums.h"
#include "const.h"
#include "shared.h"

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
				case MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME:
					SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_GAME;
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
	switch(key.keysym.sym) {
		case SDLK_DOWN:
			SharedApplicationSession.CurrentGameSession.cursorY ++;
			if (SharedApplicationSession.CurrentGameSession.cursorY > MAXIMUM_Y_COORDINATE) {
				SharedApplicationSession.CurrentGameSession.cursorY = 0;
			}
			break;
		case SDLK_UP:
			if (SharedApplicationSession.CurrentGameSession.cursorY == 0) {
				SharedApplicationSession.CurrentGameSession.cursorY = MAXIMUM_Y_COORDINATE;
			} else {
				SharedApplicationSession.CurrentGameSession.cursorY --;
			}
			break;
		case SDLK_RIGHT:
			SharedApplicationSession.CurrentGameSession.cursorX ++;
			if (SharedApplicationSession.CurrentGameSession.cursorX > MAXIMUM_X_COORDINATE) {
				SharedApplicationSession.CurrentGameSession.cursorX = 0;
			}
			break;
		case SDLK_LEFT:
			if (SharedApplicationSession.CurrentGameSession.cursorX == 0) {
				SharedApplicationSession.CurrentGameSession.cursorX = MAXIMUM_X_COORDINATE;
			} else {
				SharedApplicationSession.CurrentGameSession.cursorX --;
			}
			break;
		case SDLK_RETURN:

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
 * Renders the entire game scene, based on the application session status
 */
void renderScene() {

	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() != -1 && IMG_Init(IMG_INIT_PNG) != -1) {

		// create shared font
		SansMedium = TTF_OpenFont("OpenSans-Light.ttf", 20);
		SansSmall = TTF_OpenFont("OpenSans-Light.ttf", 14);

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

				// render game screen
				switch(SharedApplicationSession.CurrentScreen) {

					case APPLICATION_SCREENS_MENU:
						renderMenu();
						break;

					case APPLICATION_SCREENS_GAME:
						renderGame(renderer, SharedApplicationSession.CurrentGameSession);
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

//void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel) {
//	int byteperpixel = theScreen->format->BytesPerPixel;
//
//	Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x * byteperpixel;
//
//	// Adress to pixel
//	*(Uint32 *)p = pixel;
//}
////nerifrån uppåt (SW)
//void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {
//
//	int i;
//	double x = x1 - x0;
//	double y = y1 - y0;
//	double length = (double)sqrt( x*x + y*y );
//	double addx = x / length;
//	double addy = y / length;
//	x = x0;
//	y = y0;
//
//	for ( i = 0; i < length; i += 1) {
//		putpixel(Screen, x, y, pixel );
//		x += addx;
//		y += addy;
//
//	}
//}


