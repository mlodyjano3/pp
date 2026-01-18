#ifndef PHYSICS_H
#define PHYSICS_H

#include "../headers/structures.h"

int checkCollision(Hitbox player, Hitbox enemy);
void updatePlayerHitboxes(Entity* player);
void checkPlayerAttackCollisions(Entity* player, EnemiesData* enemiesData, GameState* gameState);
void cameraUpdate(Camera* camera, Entity player);
void updateFPS(GameState* gameState, int* frames, double delta);
void updateAll(Entity* player, EnemiesData* enemies, GameState* gameState, Camera* camera, double delta);
double calculateDelta(int* t1, GameState* gameState);


#endif