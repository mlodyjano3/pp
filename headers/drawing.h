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

#endif