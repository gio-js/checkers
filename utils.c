#include "enums.h"

/**
 * Get menu type description
 */
char* getMenuDescription(int menuType) {
	switch(menuType) {
		case MENU_ELEMENT_TYPE_1P_VS_2P:
			return "1P VS 2P";
		case MENU_ELEMENT_TYPE_1P_VS_CPU:
			return "1P VS CPU";
		case MENU_ELEMENT_TYPE_EXIT:
			return "Exit";
	}
	return "";
}
