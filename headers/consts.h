#ifndef CONSTS_H
#define CONSTS_H

#include <SDL.h>
#include <SDL_main.h>

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define LEVEL_WIDTH 2000

// ratio 1 do 3
#define BACKGROUND_HEIGHT (SCREEN_HEIGHT / 4) // 1/4 ekranu
#define FLOOR_HEIGHT (SCREEN_HEIGHT - BACKGROUND_HEIGHT) // 3/4 ekranu

#define BACKGROUND_WIDTH SCREEN_WIDTH
#define FLOOR_WIDTH SCREEN_WIDTH

#define FLOOR_ZERO_Y  (BACKGROUND_HEIGHT + 1)
#define FLOOR_ZERO_X 0

#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1

#endif