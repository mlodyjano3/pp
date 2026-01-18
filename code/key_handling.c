#include <SDL.h>
#include "../headers/structures.h"

void handleKey(SDL_Event event, GameState* gameState, int* frames, Entity* player, SDL_Surface* sprite, EnemiesData* enemiesData) {
    switch (event.type) {
        case SDL_KEYDOWN: {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE: {
                    gameState->quit = 1;
                    break;
                }
                case SDLK_n: {
                    // przywracanie stanu poczatkowego
                    gameState->worldTime = 0.0;
                    gameState->distance = 0.0;
                    *frames = 0;
                    gameState->fpsTimer = 0.0;
                    gameState->etiSpeed = 1.0;

                    scoringInitialize(gameState);

                    // reset gracza
                    playerInitialize(player, 100, FLOOR_ZERO_Y + 10, sprite);
                    player->invicibilityTimer = 0;
                    player->isInvicible = 0;

                    // reset wrogow
                    for (int i = 0; i < enemiesData->enemies_count; i++) {
                        EntityType type = (i % 2 == 0) ? ENTITY_ENEMY_WALKER : ENTITY_ENEMY_CHARGER;
                        enemyInitialize(&enemiesData->enemies[i], sprite, player, type);
                        enemiesData->enemies[i].wasHitThisAttack = 0;
                    }
                    break;
                }
                case SDLK_p: { // tryb deva
                    gameState->devMode = !gameState->devMode;
                    break;
                }
            };
            break;
        };
        case SDL_QUIT: { // kasacja gry, okno closed
            gameState->quit = 1;
            break;
        };
    };
}