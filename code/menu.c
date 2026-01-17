#include "../headers/structures.h"
#include "../headers/drawing.h"
#include "../headers/consts.h"
#include <SDL.h>
#include <stdio.h>
#include <string.h>

typedef enum {
    MENU_START_GAME,
    MENU_HIGHSCORES,
    MENU_EXIT,
    MENU_OPTIONS_COUNT
} MenuOption;

void menuDraw(SDL_Surface* screen, SDL_Surface* charset, MenuOption selected) {
    int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
    int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);
    int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
    int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
    int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
    
    // wyczysc ekran
    SDL_FillRect(screen, NULL, czarny);
    
    // tytul
    char title[] = "BEAT'EM UP - PROJEKT";
    DrawString(screen, SCREEN_WIDTH/2 - strlen(title)*8/2, 100, title, charset);
    
    // opcje menu
    char* options[MENU_OPTIONS_COUNT] = {
        "1. Nowa gra",
        "2. Najlepsze wyniki",
        "3. Wyjscie"
    };
    
    int startY = 200;
    int spacing = 30;
    
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        // podswietl wybrana opcje
        if (i == selected) {
            DrawRectangle(screen, 
                         SCREEN_WIDTH/2 - 150, 
                         startY + i * spacing - 5,
                         300, 
                         20,
                         zolty, 
                         niebieski);
        }
        
        DrawString(screen, 
                  SCREEN_WIDTH/2 - strlen(options[i])*8/2, 
                  startY + i * spacing, 
                  options[i], 
                  charset);
    }
    
    // instrukcja
    char info[] = "Strzalki - wybor, Enter - potwierdzenie";
    DrawString(screen, SCREEN_WIDTH/2 - strlen(info)*8/2, 400, info, charset);
}

int menuRun(SDL_Surface* screen, SDL_Surface* charset, SDL_Renderer* renderer, SDL_Texture* scrtex) {
    MenuOption selected = MENU_START_GAME;
    int done = 0;
    int result = 0; // 0 = wyjscie, 1 = start gry
    
    SDL_Event event;
    
    while (!done) {
        // rysuj menu
        menuDraw(screen, charset, selected);
        
        // wyswietl
        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // obsluga eventow
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                done = 1;
                result = 0;
            }
            
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (selected > 0) {
                            selected--;
                        }
                        break;
                    
                    case SDLK_DOWN:
                        if (selected < MENU_OPTIONS_COUNT - 1) {
                            selected++;
                        }
                        break;
                    
                    case SDLK_RETURN:
                    case SDLK_SPACE:
                        if (selected == MENU_START_GAME) {
                            done = 1;
                            result = 1; // start gry
                        } else if (selected == MENU_HIGHSCORES) {
                            // TODO: wyswietl wyniki
                            printf("Wyniki - jeszcze nie zaimplementowane\n");
                        } else if (selected == MENU_EXIT) {
                            done = 1;
                            result = 0; // wyjscie
                        }
                        break;
                    
                    case SDLK_ESCAPE:
                        done = 1;
                        result = 0;
                        break;
                }
            }
        }
        
        SDL_Delay(16); // ~60 FPS
    }
    
    return result;
}