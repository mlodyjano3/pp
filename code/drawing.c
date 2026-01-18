#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include <SDL.h>
#include <SDL_main.h>
#include <../headers/structures.h>
#include <../headers/combos.h>

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
    else if (player->currentState == ENTITY_ATTACK_HEAVY) row = 14;// 65 to ostatni index aatkow w pliku

	//animacje dla combo
	else if (player->currentState == ENTITY_COMBO_TRIPLE_LIGHT) row = 16;
    else if (player->currentState == ENTITY_COMBO_TRIPLE_HEAVY) row = 17;
    else if (player->currentState == ENTITY_COMBO_MIXED) row = 18;
    else if (player->currentState == ENTITY_DASHING) row = 19;

    else if (player->currentState == ENTITY_HIT) row = 2;
    else if (player->currentState == ENTITY_CHARGING) row = 11;

    src.x = player->frame * frameW;
    src.y = row * frameH;
    src.w = frameW;
    src.h = frameH;

    dest.w = (int)(frameW * scale);
    dest.h = (int)(frameH * scale);

    dest.x = x - dest.w / 2;
    dest.y = y;

    if (player->isInvicible) {
        int visible = ((int)(player->invicibilityTimer * 10)) % 2;
        if (!visible) {
            return;
        }
    }

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

void DrawHealthBar(SDL_Surface* screen, int x, int y, int width, int height, 
                   int currentHP, int maxHP) {
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
    
    // obliczanie ile hp ma
    float hpPercent = (float)currentHP / (float)maxHP;
    if (hpPercent < 0) hpPercent = 0;
    if (hpPercent > 1) hpPercent = 1;
    
    // hp kolor
    int fillColor = zielony;
    if (hpPercent < 0.25f) {
        fillColor = czerwony;
    } else if (hpPercent < 0.5f) {
        fillColor = zolty;
    }
    
    // tlo
    DrawRectangle(screen, x, y, width, height, czarny, czerwony);
    
    // wypelnienie
    int currentWidth = (int)(width * hpPercent);
    if (currentWidth > 0) {
        DrawRectangle(screen, x, y, currentWidth, height, czarny, fillColor);
    }
};

void DrawDevMode(SDL_Surface* screen, SDL_Surface* charset, Entity* player, GameState* gameState) {
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    
    char text[256];
    
    // tlo dla dev info
    DrawRectangle(screen, 10, 45, 300, 150, zielony, czarny);
    
    DrawString(screen, 15, 50, "=== DEV MODE ===", charset);
    // stan gracza
    sprintf(text, "Stan: %d", player->currentState);
    DrawString(screen, 15, 65, text, charset);
    
    // aktywne combo
    sprintf(text, "Combo: %s", GetComboName(player->activeCombo));
    DrawString(screen, 15, 75, text, charset);
    
    // bufor inputow
    sprintf(text, "Bufor (%d):", player->inputBuffer.count);
    DrawString(screen, 15, 85, text, charset);
    
    for (int i = 0; i < player->inputBuffer.count && i < 5; i++) {
        char* inputName = "?";
        switch (player->inputBuffer.buffer[i].type) {
            case INPUT_ATTACK_LIGHT: {
                inputName = "Q"; break;
            };
            case INPUT_ATTACK_HEAVY: {
                inputName = "E";
                 break;
            }
            case INPUT_MOVE_LEFT:{
                inputName = "<"; 
                break;
            };
            case INPUT_MOVE_RIGHT: {
                inputName = ">"; 
                break;
            };
            case INPUT_MOVE_UP: {
                inputName = "^"; 
                break;
            };
            case INPUT_MOVE_DOWN: {
                inputName = "v"; 
                break;
            };
            default:{
                inputName = "?"; break;
            };
        }
        sprintf(text, " [%d] %s (%.2fs)", i, inputName, 
                player->inputBuffer.buffer[i].timestamp);
        DrawString(screen, 15, 95, text, charset);
    }
    
    // multiplier i punkty
    sprintf(text, "Punkty: %.0f", gameState->score);
    DrawString(screen, 15, 105, text, charset);
    sprintf(text, "Multiplier: x%.1f", gameState->currentMultiplier);
    DrawString(screen, 15, 115, text, charset);
    sprintf(text, "Combo timer: %.1fs", gameState->comboTimer);
    DrawString(screen, 15, 130, text, charset);
}


void DrawAttackRange(SDL_Surface* screen, Entity* entity, Camera camera, 
                    int attackWidth, int attackHeight, Uint32 color) {
    // zasieg ataku
    int range_x = entity->position.x;
    int range_y = entity->position.y;
    
    // offset (zalezy od kierunkuy patrzenia)
    if (entity->facingLeft) {
        range_x -= attackWidth;
    } else {
        range_x += entity->measurements.w;
    };
    
    // górna linia
    DrawLine(screen,range_x - camera.position.x, 
            range_y - attackHeight - camera.position.y,
            attackWidth, 1, 0, color);
    
    // dolna linia
    DrawLine(screen, range_x - camera.position.x, 
            range_y - camera.position.y,
            attackWidth, 1, 0, color);
    
    // lewa linia
    DrawLine(screen, 
            range_x - camera.position.x, 
            range_y - attackHeight - camera.position.y,
            attackHeight, 0, 1, color);
    
    // prawa linia
    DrawLine(screen, range_x + attackWidth - camera.position.x, 
            range_y - attackHeight - camera.position.y,
            attackHeight, 0, 1, color);
}

void DrawCircleRange(SDL_Surface* screen, int centerX, int centerY, 
                     int radius, Uint32 color) {
    int circlepoints = 32;
    
    for (int i = 0; i < circlepoints; i++) {
        float angle1 = (i * 2 * M_PI) / circlepoints;
        float angle2 = ((i + 1) *2 * M_PI) / circlepoints;
        
        int x1 = centerX + (int)(cos(angle1) * radius);
        int y1 = centerY + (int)(sin(angle1) * radius);
        int x2 = centerX + (int)(cos(angle2) * radius);
        int y2 = centerY + (int)(sin(angle2) * radius);
        
        // linie miedzy punktami
        int dx = x2 - x1;
        int dy = y2 - y1;
        int absdx = abs(dx);
        int absdy = abs(dy);
        int steps;
        if (absdx > absdy) {
            steps = absdx;
        } else { steps = absdy; };
        
        if (steps > 0) {
            float xInc = dx / (float)steps;
            float yInc = dy / (float)steps;
            float x = x1;
            float y = y1;
            
            for (int j = 0; j <= steps; j++) {
                DrawPixel(screen, (int)x, (int)y, color);
                x += xInc;
                y += yInc;
            }
        }
    }
};