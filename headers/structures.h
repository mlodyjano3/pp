#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <SDL.h>
#include <SDL_main.h>

typedef struct {
    float x, y;
} Position;

typedef struct {
    float x, y;
} Direction;

typedef enum {
    ENTITY_PLAYER,
    ENTITY_ENEMY_WALKER,
    ENTITY_ENEMY_CHARGER
} EntityType;

typedef struct {
    Position position;
    int w, h; // szerokosc i wys
    Direction direction;
    float speed; 
    int health;
    int maxHealth;
    int onGround;
    int state;
    int frame;
    int facingLeft;
    EntityType type;
    SDL_Surface* tex;
} Entity;

typedef struct { 
    Position position; // pozycja kamery
    int w, h;
} Camera;

typedef struct {
    double worldTime;
    double fpsTimer;
    double fps;
    int frames;
    double distance;
    double etiSpeed;
    int quit;
} GameState;

#endif