#ifndef PLAYER_H
#define PLAYER_H

void playerInitialize(Entity* player, float x, float y, SDL_Surface* tex);
void playerUpdate(Entity* player, double delta, EnemiesData* enemiesData, GameState* gameState);
void UpdatePlayerAnimation(Entity *player, double deltaTime);
void isPlayerAlive(Entity *player, GameState *gameState);
void playerDetectKeys(const Uint8* state, Entity* player, const Uint8* prevStateBuffer, float currentTime);
int playerIsAttacking(Entity* player);
void playerUpdateTimers(Entity *player, double delta );
void playerHandleBasicAttacks(Entity *player ,const Uint8* state);
void playerRespectBoundaries(Entity* player, float currentHeight);
void playerHandleMove(Entity *player, const Uint8* state, double delta);
void playerhandleJump(Entity *player, const Uint8* state, double delta);


#endif