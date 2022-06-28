#include <SDL.h>
#include "debugmalloc.h"

/* ablak letrehozasa */
void sdl_init(char const* felirat, int szeles, int magas, SDL_Window** pwindow, SDL_Renderer** prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Nem indithato az SDL!: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window* window = SDL_CreateWindow(felirat, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
    if (window == NULL) {
        SDL_Log("Nem hozhato letre az ablak!: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Nem hozhato letre a renderer!: %s", SDL_GetError());
        exit(1);
    }

    *pwindow = window;
    *prenderer = renderer;

}
