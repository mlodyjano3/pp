#include <SDL.h>
#include "../headers/structures.h"

void enemyInitialize(Entity* enemy, SDL_Surface* tex, Entity* player, EntityType enemyType);
void enemyAttack(Entity* enemy, Entity* player, double delta);
void enemyUpdatePosition(Entity* enemy, Entity* player, double delta);
void enemiesUpdate(EnemiesData* enemiesData, Entity* player, double delta);
float calcDistance(Entity player, Entity enemy);