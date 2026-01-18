#include <SDL.h>
#include "../headers/structures.h"

void enemyInitialize(Entity* enemy, SDL_Surface* tex, Entity* player, EntityType enemyType);
void enemyAttack(Entity* enemy, Entity* player, double delta, GameState* gameState);
void enemyUpdatePosition(Entity* enemy, Entity* player, double delta);
void enemiesUpdate(EnemiesData* enemiesData, Entity* player, double delta, GameState* gameState);
float calcDistance(Entity player, Entity enemy);
void chargerCharge(Entity* enemy, double delta);
void chargerNoCharge(Entity* enemy, double delta, float distanceToPlayer, float dx, float dy);
void enemyBoundaries(Entity *enemy);
