#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include <SDL.h>
#include <SDL_main.h>
#include <../headers/structures.h>

void DrawString(SDL_Surface *screen, int x, int y, const char *text, SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};



void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
};

void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y, float scale) {
	SDL_Rect dest; // gdzie bedzie rysowany sprite

	// set wartosci
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;

	dest.w = sprite->w;
	dest.h = sprite->h;
	
	SDL_BlitScaled(sprite, NULL, screen, &dest);
};

void DrawEntityScaledAnimated(SDL_Surface *screen, SDL_Surface *sprite, int x, int y, 
	float scale, Entity *player) {
	SDL_Rect src;
    SDL_Rect dest;


	int frameW = 64; 
    int frameH = 64;

	int row=0;
	if (player->currentState == ENITY_IDLE) row = 2; 
    else if (player->currentState == ENTITY_WALKING) row = 11; //
    else if (player->currentState == ENTITY_ATTACK_LIGHT) row = 15;
    else if (player->currentState == ENTITY_ATTACK_HEAVY) row = 14;// 65 to ostatni index aatkow w plik

	//animacje dla combo
	else if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT) row = 16;
    else if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) row = 17;
    else if (player->currentState == ENTITY_COMBO_MIXED) row = 18;
    else if (player->currentState == ENTITY_DASHING) row = 19;

    src.x = player->frame * frameW;
    src.y = row * frameH;
    src.w = frameW;
    src.h = frameH;

    dest.w = (int)(frameW * scale);
    dest.h = (int)(frameH * scale);

    dest.x = x - dest.w / 2;
    dest.y = y;

    SDL_BlitScaled(sprite, &src, screen, &dest);
};

void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};


void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
		int i;
		DrawLine(screen, x, y, k, 0, 1, outlineColor);
		DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
		DrawLine(screen, x, y, l, 1, 0, outlineColor);
		DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
		for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};
	
void DrawTexture(
	SDL_Renderer *renderer, 
	SDL_Texture *texture, 
	int x, 
	int y, 
	int width, 
	int height, 
	float scale, 
	float rotation_angle,
	const SDL_RendererFlip flip) { 
		
	SDL_Rect destination;
			
	destination.x = x;
	destination.y = y - height * scale; 
			
	destination.w = width *scale;
	destination.h = height * scale;
		
	SDL_RenderCopyEx(renderer, texture, NULL, &destination, rotation_angle, NULL, flip);		
};


/*
void drawAllEnemies(EnemiesData* enemiesData) {
	for (int i =0; i < enemiesData->enemies_count; i++) {
		Entity* currentEnemy = &enemiesData->enemies[i];
		if (currentEnemy->health.health > 0) {
			int color = (currentEnemy->type == ENTITY_ENEMY_WALKER) ? niebieski : czerwony;
        
			DrawRectangle(screen,
				(int)currentEnemy->position.x - camera.position.x,
				(int)currentEnemy->position.y - enemy->measurements.h,
				enemy->measurements.w,
				enemy->measurements.h,
				czarny,   // outline
				color     // fill
			);

		}
	};
}
*/