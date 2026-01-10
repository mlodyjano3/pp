#include "SDL2-2.0.10/include/SDL_video.h"
#include <stdbool.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#include "./SDL2-2.0.10/include/SDL.h"

int main (int argo, char *argv[]) {
    SDL_Event event;
    SDL_Surface *screen;
    SDL_Texture *scrtex;
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    };

    int status = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    if (status != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    };

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowTitle(window, "Beat'em up by Jan Krol");

    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Surface *playerSprite = SDL_LoadBMP("./eti.bmp");
    if (playerSprite == NULL) {
        printf("SDL_LoadBMP error: %s\n", SDL_GetError());
    };

    int playerX = SCREEN_WIDTH / 2;
    int playerY = SCREEN_HEIGHT / 2;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                SDL_Keycode key_code = event.key.keysym.sym;
                if (key_code == SDLK_ESCAPE) {
                    quit = 1;
                } else if (key_code == SDLK_UP) {
                    playerY -= 10;
                } else if (key_code == SDLK_DOWN) {
                    playerY += 10;
                } else if (key_code == SDLK_LEFT) {
                    playerX -= 10;
                } else if (key_code == SDLK_RIGHT) {
                    playerX += 10;
                } else if (key_code == SDLK_n) {
                    // new game
                };
            };
        };

        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

        SDL_Rect dest;
        dest.x = playerX - playerSprite->w / 2;
        dest.y = playerY - playerSprite->h / 2;
        dest.w = playerSprite->w;
        dest.h = playerSprite->h;
        SDL_BlitSurface(playerSprite, NULL, screen, &dest);

        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
    };

    SDL_FreeSurface(playerSprite);
    SDL_FreeSurface(screen);
    SDL_DestroyTexture(scrtex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}