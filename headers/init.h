#ifndef INIT_H
#define INIT_H

#include <SDL.h>

int configureSDL(SDL_Window** window, SDL_Renderer** renderer);
void initVariables(GameState* gameState, Camera* camera);
void initColors(int* czarny, int* zielony, int* czerwony, int* niebieski, int* groundColor, int* skyColor, SDL_Surface* screen)

#endif