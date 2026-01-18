#include <SDL.h>
#include <SDL_main.h>
#include "../headers/consts.h"
#include "../headers/structures.h"

#include <stdio.h>

int configureSDL(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    };

    // tworzy okno i renderere
    int rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, window, renderer);
    if (rc != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(*renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(*renderer, 0, 0, 0, 255);
    SDL_SetWindowTitle(*window, "Projekt etap 2 Jan Krol");
    SDL_ShowCursor(SDL_DISABLE);

    return 0;
};

void initVariables(GameState* gameState, Camera* camera) {
    gameState->fpsTimer = 0;
	gameState->fps = 0;
	gameState->worldTime = 0;
	gameState->distance = 0;
	gameState->etiSpeed = 1;
	gameState->quit = 0;

    Camera camera;
	camera->position.x = 0;
	camera->position.y = 0;
	camera->w = SCREEN_WIDTH;
	camera->h = SCREEN_HEIGHT;
};

void initColors(int* czarny, int* zielony, int* czerwony, int* niebieski, int* groundColor, int* skyColor, SDL_Surface* screen) {
    czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	groundColor = SDL_MapRGB(screen->format, 0x22, 0x88, 0x22);
	skyColor = SDL_MapRGB(screen->format, 0x88, 0xCC, 0xFF);
};