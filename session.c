#include <string.h>
#include "models.h"
#include "shared.h"

/**
 * Generates a new local game session
 */
void initializeSharedSession() {
	memset(&SharedApplicationSession, 0, sizeof(t_ApplicationSession));
}
