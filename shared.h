#include "models.h"
#include "SDL2/SDL_ttf.h"

#ifndef SHARED_H
#define SHARED_H

/**
 * Contains the current application game session
 */
extern t_ApplicationSession SharedApplicationSession;

// fonts
extern TTF_Font *SansSmall;
extern TTF_Font *SansMedium;
extern TTF_Font *SansLarge;

#endif
