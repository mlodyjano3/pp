#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifdef __APPLE__
#include <SDL.h>
#else
#include "./SDL2-2.0.10/include/SDL.h"
#endif

typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_WALKER,
    ENTITY_ENEMY_CHARGER
} EntityType;

typedef struct {
    float x, y;
    int w, h;
    float dx, dy;
    float speed;
    int health;
    int maxHealth;
    int onGround;
    int state;
    int frame;
    int facingLeft;
    EntityType type;
    SDL_Texture* tex;
} Entity;

typedef struct {
    float x, y;
    int w, h;
} Camera;

#endif