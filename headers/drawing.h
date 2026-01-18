#ifndef DRAWING_H
#define DRAWING_H

#include<math.h>
#include<stdio.h>
#include<string.h>

#include <SDL.h>
#include <SDL_main.h>


void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset);
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y);
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color);
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color);
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k, Uint32 outlineColor, Uint32 fillColor);
void DrawTexture(
	SDL_Renderer *renderer, 
	SDL_Texture *texture, 
	int x, 
	int y, 
	int width, 
	int height, 
	float scale, 
	float rotation_angle,
	const SDL_RendererFlip flip);
void DrawEntityScaledAnimated(SDL_Surface *screen, SDL_Surface *sprite, int x, int y, 
	float scale, Entity *player);
void DrawHealthBar(SDL_Surface* screen, int x, int y, int width, int height, 
                	int currentHP, int maxHP);
void DrawDevMode(SDL_Surface* screen, SDL_Surface* charset, Entity* player, GameState* gameState);
void DrawAttackRange(SDL_Surface* screen, Entity* entity, Camera camera, 
                     int attackWidth, int attackHeight, Uint32 color);
void DrawCircleRange(SDL_Surface* screen, int centerX, int centerY, 
                     int radius, Uint32 color);

#endif