#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>

#include "../headers/structures.h"
#include "../headers/drawing.h"
#include "../headers/loadfiles.h"
#include "../headers/consts.h"
#include "../headers/player.h"
#include "../headers/init.h"
#include "../headers/enemies.h"
#include "../headers/points.h"
#include "../headers/menu.h"
#include "../headers/physics.h"
#include "../headers/key_handling.h"

#include <SDL.h>
#include <SDL_main.h>


int main(int argc, char **argv) {
	// inicjalizacje wszelkie

	SDL_Event event;
	SDL_Surface *eti;
	SDL_Surface *charset;

	AppResources appResources;

	SDL_Rect skyRectangle = {0, 0, SCREEN_WIDTH, BACKGROUND_HEIGHT};
	SDL_Rect groundRectangle = { 0,FLOOR_ZERO_Y, SCREEN_WIDTH, FLOOR_HEIGHT};

	Camera camera;
	EnemiesData enemiesData;
	GameState gameState;
	Entity player;

	srand(time(NULL));

	int configurationStatus = configureSDL(&appResources.window, &appResources.renderer);
	if (configurationStatus != 0) {
		return 1;
	};

	appResources.screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	appResources.scrtex = SDL_CreateTexture(appResources.renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH, SCREEN_HEIGHT);


	int czarny, zielony, czerwony, niebieski, groundColor, skyColor;
	initColors(&czarny, &zielony, &czerwony, &niebieski, &groundColor, &skyColor, &appResources.screen);


	if (LoadFiles(&appResources.screen, &charset, &eti, appResources.window, appResources.renderer, appResources.scrtex, &appResources.sprite) != 0) {
        printf("Blad ladowania plikow!\n");
        return 1; 
    }
	SDL_SetColorKey(charset, SDL_TRUE, 0x000000);

	playerInitialize(
		&player, 
		100,
		FLOOR_ZERO_Y + 10,
		appResources.sprite
	);

	// napraw
	enemiesData.enemies_count = 5;

	for (int i = 0; i < enemiesData.enemies_count; i++) {
		EntityType type = (i % 2 == 0) ? ENTITY_ENEMY_WALKER : ENTITY_ENEMY_CHARGER;
		enemyInitialize(&enemiesData.enemies[i], appResources.sprite, &player, type);
	};

	int t1 = SDL_GetTicks();
	int frames = 0;
	
	initVariables(&gameState, &camera);

	scoringInitialize(&gameState);
    
    // inicjalizacja wrogow 
    for (int i = 0; i < enemiesData.enemies_count; i++) {
        enemiesData.enemies[i].wasHitThisAttack = 0;
    }
    
	// menu dla gracza
    int startGame = menuRun(appResources.screen, charset, appResources.renderer, appResources.scrtex);
    if (!startGame) {
        // gracz wybralem wyjscie z menu
        gameState.quit = 1;
	};

    //
	// glowna petla
    //
	while(!gameState.quit) {
		double delta = calculateDelta(&t1, &gameState);
		gameState.worldTime += delta;

		updateAll(&player, &enemiesData, &gameState, &camera, delta);

		drawAll(
			&appResources.screen, &charset, skyRectangle, groundRectangle, 
			&player, &camera, &enemiesData, &gameState, 
			skyColor, groundColor, niebieski, czerwony, zielony, czarny
		);

		displayFrame(&appResources.renderer, &appResources.scrtex, &appResources.screen);
		
		// aktualizacja fpsow
		updateFPS(&gameState, &frames, delta);

		// obsluga przyciskow
		while (SDL_PollEvent(&event)) {
            handleKey(
				event, &gameState, &frames, &player, &appResources.sprite, &enemiesData
			);
		};
		frames++;
	};
	// zwolnienie resources
	SDL_FreeSurface(charset);
	SDL_FreeSurface(appResources.screen);
	SDL_DestroyTexture(appResources.scrtex);
	SDL_DestroyRenderer(appResources.renderer);
	SDL_DestroyWindow(appResources.window);

	SDL_Quit();
	return 0;
};