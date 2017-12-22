#include "SDL2/SDL.h"

#ifndef CONST_H
#define CONST_H

// number of menu items
#define MENU_COUNT 2

// screen width
#define SCREEN_WIDTH 640

// screen height
#define SCREEN_HEIGHT 480

// number of table cells
#define CELL_NUMBERS 64

// max x for the piece position
#define MAXIMUM_X_COORDINATE 7

// max y for the piece position
#define MAXIMUM_Y_COORDINATE 7

// 12 pieces for each player
#define PLAYER_PIECE_NUMBERS 12

// 12 pieces for each player (24)
#define TABLE_PIECE_NUMBERS 24

// score weight for a movement take
#define MOVEMENT_TAKE_SCORE_WEIGHT 1

// score weight for a movement king
#define MOVEMENT_KING_SCORE_WEIGHT 0.5

// colors
static const SDL_Color COLOR_FIREBRICK = (SDL_Color){ 70, 13, 13 };
static const SDL_Color COLOR_WHITE = (SDL_Color){ 255, 255, 255 };

#endif
