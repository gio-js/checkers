#include "functions.h"

/**
 * main
 */
int WinMain(int argc, char** argv) {
	// initialize shared session
	initializeSharedSession();

	// render the main application scene
	renderScene();

	return 0;
}
