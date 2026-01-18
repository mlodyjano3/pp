#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include <SDL.h>
#include <SDL_main.h>
#include "../headers/structures.h"
#include "../headers/combos.h"

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
    int range_x = entity->attacking_hitboxes.x;
    int range_y = entity->attacking_hitboxes.y;
    
    int screen_x = range_x - camera.position.x;
    int screen_y = range_y - camera.position.y;
    
    for (int i = 0; i < 4; i++) {
        DrawLine(screen, screen_x, screen_y + attackHeight - i, attackWidth, 1, 0, color);
    }
    
    for (int i = 0; i < 8; i++) {
        DrawPixel(screen, screen_x, screen_y + attackHeight - 4 - i, color);
        DrawPixel(screen, screen_x + attackWidth, screen_y + attackHeight - 4 - i, color);
    }
};

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

void shouldDrawRanges(int* shouldDrawRanges, GameState gameState) {
    *shouldDrawRanges = 0;
    if (RANGES_DISPLAYED_DEV_ONLY == 0) {
        *shouldDrawRanges = 1;
    } else if (RANGES_DISPLAYED_DEV_ONLY == 1 && gameState.devMode) {
        *shouldDrawRanges = 1; // dev mode rysowanie
    };
};

void drawingRanges(Entity player, Camera *camera, GameState gameState, SDL_Surface* screen) {
    int attackW = 0;
    int attackH = 0;

    if (player.currentState == ENTITY_ATTACK_LIGHT) {
        attackW = LIGHT_ATTACK_WIDTH;
        attackH = LIGHT_ATTACK_HEIGHT;
    } else if (player.currentState == ENTITY_ATTACK_HEAVY) {
        attackW = HEAVY_ATTACK_WIDTH;
        attackH = HEAVY_ATTACK_HEIGHT;
    } else if (player.currentState == ENTITY_COMBO_TRIPLE_LIGHT) {
        attackW = LIGHT_ATTACK_WIDTH + 20;
        attackH = LIGHT_ATTACK_HEIGHT + 10;
    } else if (player.currentState == ENTITY_COMBO_TRIPLE_HEAVY) {
        attackW = HEAVY_ATTACK_WIDTH + 30;
        attackH = HEAVY_ATTACK_HEIGHT + 20;
    } else if (player.currentState == ENTITY_COMBO_MIXED) {
        attackW = HEAVY_ATTACK_WIDTH + 10;
        attackH = HEAVY_ATTACK_HEIGHT + 10;
    };

    if (attackW > 0) {
        int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
        DrawAttackRange(screen, &player, *camera, attackW, attackH, zolty);
    };
};

void drawEnemies(
    EnemiesData* enemiesData, Camera* camera, 
    SDL_Surface* screen, int shouldDrawRangesBool,
    int niebieski, int czerwony, int zielony, int czarny) {
    for (int i = 0; i < enemiesData->enemies_count; i++) {
        Entity* enemy = &enemiesData->enemies[i];
        
        if (enemy->health.health > 0) {
            int fillColor = niebieski;
            
            if (enemy->type == ENTITY_ENEMY_WALKER) {
                fillColor = niebieski;
            } else if (enemy->type == ENTITY_ENEMY_CHARGER) {
                if (enemy->chargerData.isCharging) {
                    fillColor = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
                } else {
                    fillColor = czerwony;
                };
            };
            
            float currentHeight = enemy->measurements.h * enemy->scale;
            float currentWidth = enemy->measurements.w * enemy->scale;
            
            DrawRectangle(screen,
                (int)enemy->position.x - camera->position.x,
                (int)(enemy->position.y - currentHeight),
                (int)currentWidth,
                (int)currentHeight,
                czarny,
                fillColor
            );
            
            int hpBarWidth = (int)currentWidth;
            int currentHpWidth = (int)(hpBarWidth * ((float)enemy->health.health / enemy->health.maxHealth));
            if (currentHpWidth < 0) {
                currentHpWidth = 0;
            };
            if (currentHpWidth > hpBarWidth) {
                currentHpWidth = hpBarWidth;
            };

            
            DrawRectangle(screen,
                (int)enemy->position.x - camera->position.x,
                (int)(enemy->position.y - currentHeight - 8),
                currentHpWidth,
                4,
                czarny,
                zielony
            );
            
            if (shouldDrawRangesBool) {
                int rangeColor = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF);
                
                if (enemy->type == ENTITY_ENEMY_WALKER) {
                    DrawCircleRange(screen,
                        (int)enemy->position.x - camera->position.x + (int)currentWidth/2,
                        (int)(enemy->position.y - currentHeight/2),
                        WALKER_ATTACK_RANGE,
                        rangeColor
                    );
                } else if (enemy->type == ENTITY_ENEMY_CHARGER) {
                    int detectColor = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);
                    DrawCircleRange(screen,
                        (int)enemy->position.x - camera->position.x + (int)currentWidth/2,
                        (int)(enemy->position.y - currentHeight/2),
                        CHARGER_DETECT_RANGE,
                        detectColor
                    );
                    
                    DrawCircleRange(screen,
                        (int)enemy->position.x - camera->position.x + (int)currentWidth/2,
                        (int)(enemy->position.y - currentHeight/2),
                        CHARGER_ATTACK_RANGE,
                        rangeColor
                    );
                }
            }
        }
    }
};



void drawWorld(SDL_Surface* screen, Entity* player, Camera* camera, EnemiesData* enemiesData, GameState* gameState, SDL_Rect skyRectangle, SDL_Rect groundRectangle, int skyColor, int groundColor, int niebieski, int czerwony, int zielony, int czarny) {
    // podstawa + niebo
    SDL_FillRect(screen, &skyRectangle, skyColor);
    SDL_FillRect(screen, &groundRectangle, groundColor);

    // rysowanie gracza
    player->scale = BASE_PLAYER_SIZE + (player->position.y / (float)SCREEN_HEIGHT);
    
    DrawEntityScaledAnimated(screen, player->tex, 
        (int)player->position.x - camera->position.x + (player->measurements.w / 2),
        (int)player->position.y - player->position.z,
        player->scale,
        player
    );

    int shouldDrawRangesBool;
    shouldDrawRanges(&shouldDrawRangesBool, *gameState);
    
    if (shouldDrawRangesBool) {
        drawingRanges(*player, camera, *gameState, screen);
    };

    drawEnemies(enemiesData, camera, screen, shouldDrawRangesBool, niebieski, czerwony, zielony, czarny);
};

void drawInterface(
    SDL_Surface* screen, SDL_Surface* charset, Entity player, GameState* gameState,
    int czerwony, int niebieski, int zielony, int czarny) {
    char text[128];
    // interfejs
    DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 60, czerwony, niebieski);
    
    //pasek hp gracza
    DrawHealthBar(screen, 10, 10, 200, 15, player.health.health, player.health.maxHealth);
    sprintf(text, "HP: %d/%d", player.health.health, player.health.maxHealth);
    DrawString(screen, 10, 28, text, charset);
    
    // punkty i multiplier
    sprintf(text, "Punkty %.0f", gameState->score);
    DrawString(screen, 220, 10, text, charset);
    
    sprintf(text, "Combo: x%.1f", gameState->currentMultiplier);
    DrawString(screen, 220, 20, text, charset);
    
    // czas
    sprintf(text, "Czas: %.1lf s  | ilosc FPS = %.0lf", gameState->worldTime, gameState->fps);
    DrawString(screen, 220, 30, text, charset);
    
    // wymagania
    DrawString(screen, 10, 45, "Zaimplementowane wymagania: 1, 2, 3, 4, A, B, C, D, E, G", charset);
};

void drawAll(SDL_Surface* screen, SDL_Surface* charset, SDL_Rect skyRect, SDL_Rect groundRect, 
    Entity* player, Camera* camera, 
    EnemiesData* enemiesData, GameState* gameState, 
    int skyColor, int groundColor, 
    int niebieski, int czerwony, int zielony, int czarny) {
    
    drawWorld(
        screen, player, camera, enemiesData, gameState, 
        skyRect, groundRect, skyColor, groundColor, 
        niebieski, czerwony, zielony, czarny);

    drawInterface(screen, charset, *player, gameState, czerwony, niebieski, zielony, czarny);
    
    if (gameState->devMode) {
        DrawDevMode(screen, charset, player, gameState);
    }
};

void displayFrame(SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Surface* screen) {
    SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch); // screen -> scrtex (ram -> gpu)
	SDL_RenderCopy(renderer, scrtex, NULL, NULL); // rysuje teksture
	SDL_RenderPresent(renderer); // wyswietla frame
}