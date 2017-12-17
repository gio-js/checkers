#include <stdio.h>
#include <stdlib.h>
#include "const.h"
#include "enums.h"
#include "models.h"
#include "SDL2/SDL.h"


void putpixel(SDL_Surface *theScreen, int x, int y, Uint32 pixel) {
    int byteperpixel = theScreen->format->BytesPerPixel;

    Uint8 *p = (Uint8*)theScreen->pixels + y * theScreen->pitch + x * byteperpixel;

	// Adress to pixel
    *(Uint32 *)p = pixel;
}
//nerifrån uppåt (SW)
void drawLine(SDL_Surface *Screen, int x0, int y0, int x1, int y1, Uint32 pixel) {

	int i;
    double x = x1 - x0; 
	double y = y1 - y0; 
	double length = (double)sqrt( x*x + y*y ); 
	double addx = x / length; 
	double addy = y / length; 
	x = x0; 
	y = y0; 
	
	for ( i = 0; i < length; i += 1) { 
		putpixel(Screen, x, y, pixel ); 
		x += addx; 
		y += addy; 
		
	} 
}

int main(int argc, char** argv) {

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        SDL_Window* window = NULL;
        SDL_Renderer* renderer = NULL;

        if (SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer) == 0) {
            SDL_bool done = SDL_FALSE;

            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
                SDL_RenderPresent(renderer);

                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    }
                }
            }
        }

        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
    }
    SDL_Quit();
    return 0;


}

/*int WinMain( int argc, char* args[] )
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    return 1;
  }
  atexit(SDL_Quit);
    
    return 0;    
}*/


/*int main( int argc, char* args[] )
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0) {
    fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError());
    return 1;
  }
  atexit(SDL_Quit);

  return 0;
}*/
