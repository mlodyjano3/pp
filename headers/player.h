#ifndef PLAYER_H
#define PLAYER_H

void playerInitialize(Entity* player, float x, float y, SDL_Surface* tex);
void playerUpdate(Entity* player, double delta, EnemiesData* enemiesData, GameState* gameState);
void UpdatePlayerAnimation(Entity *player, double deltaTime);
void isPlayerAlive(Entity *player, GameState *gameState);

#endif