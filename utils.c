#include "enums.h"

/**
 * Get menu type description
 */
char* getMenuDescription(int menuType) {
	switch(menuType) {
		case MENU_ELEMENT_TYPE_SINGLE_PLAYER_GAME:
			return "Single player";
		case MENU_ELEMENT_TYPE_EXIT:
			return "Exit";
	}
	return "";
}
