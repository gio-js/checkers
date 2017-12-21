#ifndef ENUMS_H
#define ENUMS_H

/**
 * Application screen types
 */
enum APPLICATION_SCREENS {
    APPLICATION_SCREENS_MENU = 0,
    APPLICATION_SCREENS_GAME,
    APPLICATION_SCREENS_EXIT
};

/**
 * Application menu elements
 */
enum MENU_ELEMENT_TYPE {
    MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME = 0,
    MENU_ELEMENT_TYPE_EXIT
};

/**
 * CELL type
 */
enum CELL_TYPE {
    CELL_TYPE_WHITE = 0,
    CELL_TYPE_BLACK
};

/**
 * Player type
 */
enum PLAYER_TYPE {
    PLAYER_TYPE_HUMAN = 0,
    PLAYER_TYPE_COMPUTER
};

/**
 * Player main group color
 */
enum PLAYER_COLOR {
    PLAYER_COLOR_WHITE = 0,
    PLAYER_COLOR_BLACK
};

/**
 * Pawn type
 */
enum PAWN_TYPE {
    PAWN_TYPE_SIMPLE_PIECE = 0,
    PAWN_TYPE_KING
};

#endif
