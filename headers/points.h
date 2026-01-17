#ifndef POINTS_H
#define POINTS_H

#include "../headers/structures.h"

void scoringInitialize(GameState* gameState);
void scoringUpdate(GameState* gameState, double delta);
void scoringAddPoints(GameState* gameState, Entity* player, int basePoints);
void scoringOnEnemyHit(GameState* gameState, Entity* player);
void scoringOnPlayerHit(GameState* gameState);

#endif