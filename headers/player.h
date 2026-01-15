#ifndef PLAYER_H
#define PLAYER_H

void playerInitialize(Entity* player, float x, float y, SDL_Surface* tex);
void playerUpdate(Entity* player, double delta);
void UpdatePlayerAnimation(Entity *player, double deltaTime);

#endif