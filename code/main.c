#include "SDL2-2.0.10/include/SDL_video.h"
#include <stdbool.h>
#include <stdio.h>
#include "structures.h"
#include "./SDL2-2.0.10/include/SDL.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main (int argc, char *argv[]) {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init error: %s\n", SDL_GetError());
        return 1;
    }

    int status = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer);

    if (status != 0) {
        SDL_Quit();
        printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetWindowTitle(window, "Beat'em up by Jan Krol");

    Entity player;
    player.x = SCREEN_WIDTH / 2;
    player.y = SCREEN_HEIGHT / 2;
    player.speed = 200.0f;

    SDL_Surface* tempSurface = SDL_LoadBMP("./eti.bmp");
    if (tempSurface == NULL) {
        printf("SDL_LoadBMP error: %s\n", SDL_GetError());
        return 1;
    }

    player.w = tempSurface->w;
    player.h = tempSurface->h;
    player.tex = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);

    Camera camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    int t1 = SDL_GetTicks();
    int quit = 0;

    while (!quit) {
        int t2 = SDL_GetTicks();
        float delta = (t2 - t1) * 0.001f;
        t1 = t2;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    quit = 1;
                }
            }
        }

        const Uint8 *state = SDL_GetKeyboardState(NULL);

        if (state[SDL_SCANCODE_UP]) {
            player.y -= player.speed * delta;
        }
        if (state[SDL_SCANCODE_DOWN]) {
            player.y += player.speed * delta;
        }
        if (state[SDL_SCANCODE_LEFT]) {
            player.x -= player.speed * delta;
        }
        if (state[SDL_SCANCODE_RIGHT]) {
            player.x += player.speed * delta;
        }

        if (player.y < 250) player.y = 250;
        if (player.y > SCREEN_HEIGHT - player.h) player.y = SCREEN_HEIGHT - player.h;

        camera.x = player.x - (SCREEN_WIDTH / 2);
        if (camera.x < 0) camera.x = 0;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_Rect floorRect;
        floorRect.x = 0 - (int)camera.x;
        floorRect.y = 250;
        floorRect.w = 2000;
        floorRect.h = SCREEN_HEIGHT - 250;
        SDL_RenderFillRect(renderer, &floorRect);

        SDL_Rect dest;
        dest.x = (int)(player.x - camera.x) - player.w / 2;
        dest.y = (int)(player.y - camera.y) - player.h / 2;
        dest.w = player.w;
        dest.h = player.h;
        SDL_RenderCopy(renderer, player.tex, NULL, &dest);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(player.tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}