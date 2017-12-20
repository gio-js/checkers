#include "enums.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "shared.h"
#include "enums.h"

// create renderer
static SDL_Renderer* renderer = NULL;
static TTF_Font* Sans = NULL;

// colors
static const SDL_Color COLOR_WHITE = { 255, 255, 255 };
static const SDL_Color COLOR_FIREBRICK = { 70, 13, 13 };

/**
 * Draws specified text over the SDL renderer
 */
void drawText(char *text, SDL_Color color, SDL_Rect rect) {
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text, color);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_RenderCopy(renderer, Message, NULL, &rect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(Message);
}

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
			SharedApplicationSession.ScreenUserSelection --;
			if (SharedApplicationSession.ScreenUserSelection < 0) {
				SharedApplicationSession.ScreenUserSelection = MENU_COUNT - 1;
			}
			break;
		case SDLK_RETURN:
			switch(SharedApplicationSession.ScreenUserSelection) {
				case MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME:
					SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_GAME;
				case MENU_ELEMENT_TYPE_EXIT:
					SharedApplicationSession.CurrentScreen = APPLICATION_SCREENS_EXIT;
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
		if (event.type== SDL_QUIT) {
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
		TTF_SizeText(Sans,menuText,&textWidth,&textHeight); // get text size
		SDL_Rect Message_rect;
		Message_rect.x = rect.x + (width - textWidth) / 2;
		Message_rect.y = rect.y + (height - textHeight) / 2;
		Message_rect.w = textWidth;
		Message_rect.h = textHeight;

		drawText(menuText, COLOR_WHITE, Message_rect);
	}

}

/**
 * Renders the entire game scene, based on the application session status
 */
void renderScene() {

	// initialize sdl
	if (SDL_Init(SDL_INIT_VIDEO) == 0 && TTF_Init() != -1) {

		// create shared font
		Sans = TTF_OpenFont("OpenSans-Light.ttf", 20);

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

