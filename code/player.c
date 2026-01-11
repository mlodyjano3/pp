#include "../headers/structures.h"
#include "../headers/consts.h"

void playerInitialize(Entity* player, float x, float y, SDL_Surface* tex) {
    player->position.x = x; // pozycja gracza (startowa)
    player->position.y = y;

    player->w = 32; // rozmiary gracza
    player->h = 48;

    player->direction.x = 0; /// wektor ruchu
    player->direction.y = 0;

    player->speed = 200.0f; // pixele na sekunde
    player->maxHealth = 100;
    player->health = 100;
    player->type = ENTITY_PLAYER;

    player->onGround = 0;
    player->tex = tex;
};

void playerUpdate(Entity* player, double delta) {
    const Uint8* state = SDL_GetKeyboardState(NULL);
    
    // reset kierunku ruchu w kazdej klatce
    player->direction.x = 0;
    player->direction.y = 0;

    // check klawiszy ruchu + zmiana wektora
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A]) {
        player->direction.x = -1;
        player->facingLeft = 1;
    }
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D]) {
        player->direction.x = 1;
        player->facingLeft = 0;
    }
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W]) {
        player->direction.y = -1;
    }
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S]) {
        player->direction.y = 1;
    }

    // norm wektora ruchu
    player->position.x += player->direction.x * player->speed * delta;
    player->position.y += player->direction.y * player->speed * delta;


    // ograniczniki ruchu - gracz nie moze wyjsc poza mape/ekran
    if (player->position.x < 0) {
        player->position.x = 0;
    };
    if (player->position.x > LEVEL_WIDTH - player->w) {
        player->position.x = LEVEL_WIDTH - player->w;
    };
    if (player->position.y < FLOOR_ZERO_Y - player->h) {
        player->position.y = FLOOR_ZERO_Y - player->h;
    };
    if (player->position.y > FLOOR_HEIGHT - player->h) {
        player->position.y = FLOOR_HEIGHT - player->h;
    };
};