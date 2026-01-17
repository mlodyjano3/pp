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

#include <SDL.h>
#include <SDL_main.h>


int main(int argc, char **argv) {
	// inicjalizacje wszelkie

	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Surface *eti;
	SDL_Surface *sprite;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	srand(time(NULL));

	int configurationStatus;
	configurationStatus = configureSDL(&window, &renderer);
	if (configurationStatus != 0) {
		return 1;
	};

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,SCREEN_WIDTH, SCREEN_HEIGHT);

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	int groundColor = SDL_MapRGB(screen->format, 0x22, 0x88, 0x22);
	int skyColor = SDL_MapRGB(screen->format, 0x88, 0xCC, 0xFF);


	if (LoadFiles(&screen, &charset, &eti, window, renderer, scrtex, &sprite) != 0) {
        printf("Blad ladowania plikow! Upewnij sie, ze pliki .bmp sa w folderze z plikiem wykonywalnym (compiled/).\n");
        return 1; 
    }
	SDL_SetColorKey(charset, SDL_TRUE, 0x000000);

	GameState gameState;
	Entity player;

	playerInitialize(
		&player, 
		100,
		FLOOR_ZERO_Y + 10,
		sprite
	);


	// napraw
	EnemiesData enemiesData;
	enemiesData.enemies_count = 5;

	for (int i = 0; i < enemiesData.enemies_count; i++) {
		EntityType type = (i % 2 == 0) ? ENTITY_ENEMY_WALKER : ENTITY_ENEMY_CHARGER;
		enemyInitialize(&enemiesData.enemies[i], sprite, &player, type);
	};

	int t1 = SDL_GetTicks();
	int quit;
	int frames = 0;
	gameState.fpsTimer = 0;
	gameState.fps = 0;
	quit = 0;
	gameState.worldTime = 0;
	gameState.distance = 0;
	gameState.etiSpeed = 1;
	gameState.quit = 0;


	Camera camera;
	camera.position.x = 0;
	camera.position.y = 0;
	camera.w = SCREEN_WIDTH;
	camera.h = SCREEN_HEIGHT;

    //
	// glowna petla
    //
	while(!quit) {
		int t2 = SDL_GetTicks();
		double delta = (t2 - t1) * 0.001;
		t1 = t2;

		gameState.worldTime += delta;

		playerUpdate(&player, delta, &enemiesData, &gameState); // update gracza
		enemiesUpdate(&enemiesData, &player, delta);


		camera.position.x = player.position.x -(SCREEN_WIDTH / 2) + (player.measurements.h / 2); // logika positioningu kamery na osi x
		camera.position.y = SCREEN_HEIGHT / 2; // kamera zawsze pozostaje w tym samym miejscu osi y

		if (camera.position.x < 0) {
            camera.position.x = 0;
        };
		if (camera.position.x > LEVEL_WIDTH - SCREEN_WIDTH) {
            camera.position.x = LEVEL_WIDTH - SCREEN_WIDTH;
        };

		// rysowanie sceny
		SDL_Rect skyRectangle = {0, 0, SCREEN_WIDTH, BACKGROUND_HEIGHT};
		SDL_Rect groundRectangle = { 0,FLOOR_ZERO_Y, SCREEN_WIDTH, FLOOR_HEIGHT};
		SDL_FillRect(screen, &skyRectangle, skyColor);
		SDL_FillRect(screen, &groundRectangle, groundColor);

		// rysowanie gracza
		player.scale = 0.5f + (player.position.y / (float)SCREEN_HEIGHT); // skala w zaleznosci od polozenia y
		DrawEntityScaledAnimated(screen, player.tex, 
			(int)player.position.x - camera.position.x + (player.measurements.w / 2),
			(int)player.position.y - player.position.z,
			player.scale,
			&player
		);

		// rysowanie wrogow
		for (int i = 0; i < enemiesData.enemies_count; i++) {
			Entity* enemy = &enemiesData.enemies[i];
			
			if (enemy->health.health > 0) {
				int fillColor = (enemy->type == ENTITY_ENEMY_WALKER) ? niebieski : czerwony;
				
				DrawRectangle(screen,
					(int)enemy->position.x - camera.position.x,
					(int)enemy->position.y - enemy->measurements.h,
					enemy->measurements.w,
					enemy->measurements.h,
					czarny,
					fillColor
				);
				
				// Pasek HP
				int hpBarWidth = enemy->measurements.w;
				int currentHpWidth = (int)(hpBarWidth * ((float)enemy->health.health / enemy->health.maxHealth));
				
				DrawRectangle(screen,
					(int)enemy->position.x - camera.position.x,
					(int)enemy->position.y - enemy->measurements.h - 8,
					currentHpWidth,
					4,
					czarny,
					zielony
				);
			}
		}

        

		// logika fpsow
		gameState.fpsTimer += delta;
		if (gameState.fpsTimer > 0.5) {
			gameState.fps = frames * 2;
			frames = 0;
			gameState.fpsTimer -= 0.5;
		};

		// interface 
		DrawRectangle(
			screen, 
			4,
			4, 
			SCREEN_WIDTH - 8, 
			36, 
			czerwony, 
			niebieski
		);
		sprintf(text, "Szablon drugiego zadania, czas trwania = %.1lf s  %.0lf fps ", gameState.worldTime, gameState.fps);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);
		sprintf(text, "Esc - wyjscie, \030 - przyspieszenie, \031 - zwolnienie, n - nowa gra");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch); // screen -> scrtex (ram -> gpu)
		SDL_RenderCopy(renderer, scrtex, NULL, NULL); // rysuje teksture
		SDL_RenderPresent(renderer); // wyswietla frame

		// obsluga przyciskow
		while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYDOWN: {
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE: {
							quit = 1;
							break;
						} 
						case SDLK_n: {
							// przywracanie stanu poczatkowego poprzez zmiane wartosci w strukturze
							gameState.worldTime = 0.0;
							gameState.distance = 0.0;
							frames = 0;
							gameState.fpsTimer = 0.0;
							gameState.etiSpeed = 1.0;
						}
						case SDLK_p: { // tryb deva
							gameState.devMode = !gameState.devMode;
							break;
						}
					};
					break;
				};
                case SDL_QUIT: { // kasacja gry, okno closed
                    quit = 1;
                    break;
				};
        	};

		};
		frames++;
	};
	// zwolnienie resources
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};