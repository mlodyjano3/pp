#ifndef LOADFILES_H
#define LOADFILES_H

#include <SDL.h>
#include <SDL_main.h>

int LoadFiles(SDL_Surface** screen, SDL_Surface** charset, SDL_Surface** eti, 
    SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Surface** sprite);

#endif