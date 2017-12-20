/**
 * Render the checkers matrix (on the left side of the screen)
 */
void renderGameCheckers(*t_GameSession gameSession, int availableWidth, int availableHeight) {

	int index = 0;
	int startX = 
	for (index = 0; index < MAXIMUM_X_COORDINATE; index++) {
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
