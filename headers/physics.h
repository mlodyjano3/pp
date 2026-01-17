#ifndef PHYSICS_H
#define PHYSICS_H

#include "../headers/structures.h"

int checkCollision(Hitbox player, Hitbox enemy);
void updatePlayerHitboxes(Entity* player);
void checkPlayerAttackCollisions(Entity* player, EnemiesData* enemiesData, GameState* gameState);

#endif