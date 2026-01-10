#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "./SDL2-2.0.10/include/SDL.h"
#include "SDL2-2.0.10/include/SDL_keycode.h"
#include "SDL2-2.0.10/include/SDL_blendmode.h"
#include "SDL2-2.0.10/include/SDL_render.h"
#include "SDL2-2.0.10/include/SDL_surface.h"
#include "SDL2-2.0.10/include/SDL_video.h"
#include "SDL2-2.0.10/include/SDL_rect.h"

#include "util.h"
#include "structures.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

const int BACKGROUND_HEIGHT = SCREEN_HEIGHT / 4;
const int FLOOR_HEIGHT = SCREEN_HEIGHT - BACKGROUND_HEIGHT;

const int BACKGROUND_WIDTH = SCREEN_WIDTH;
const int FLOOR_WIDTH = SCREEN_WIDTH;

const int FLOOR_ZERO_Y = BACKGROUND_HEIGHT + 1;
const int FLOOR_ZERO_X = 0;

const int FLOOR_MAX_Y = SCREEN_HEIGHT;