#ifndef ENUMS_H
#define ENUMS_H

/**
 * Application screen types
 */
enum APPLICATION_SCREENS {
    APPLICATION_SCREENS_MENU,
    APPLICATION_SCREENS_GAME,
    APPLICATION_SCREENS_EXIT
};

/**
 * Application menu elements
 */
enum MENU_ELEMENT_TYPE {
    MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME,
    MENU_ELEMENT_TYPE_EXIT
};

/**
 * CELL type
 */
enum CELL_TYPE {
    CELL_TYPE_WHITE,
    CELL_TYPE_BLACK
};

/**
 * Player type
 */
enum PLAYER_TYPE {
    PLAYER_TYPE_HUMAN,
    PLAYER_TYPE_COMPUTER
};

/**
 * Player main group color
 */
enum PLAYER_COLOR {
    PLAYER_COLOR_WHITE,
    PLAYER_COLOR_BLACK
};

/**
 * Pawn type
 */
enum PAWN_TYPE {
    PAWN_TYPE_SIMPLE_PIECE,
    PAWN_TYPE_KING
};

#endif
