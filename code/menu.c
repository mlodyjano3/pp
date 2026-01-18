#include "../headers/structures.h"
#include "../headers/drawing.h"
#include "../headers/consts.h"
#include <SDL.h>
#include <stdio.h>
#include <string.h>

void menuDraw(SDL_Surface* screen, SDL_Surface* charset, MenuOption selected,
    int czerwony,int  niebieski,int zielony,int czarny) {
    int zolty = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
    
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
        // podswietla wybrana opcje
        if (i == selected) {
            DrawRectangle(screen, SCREEN_WIDTH /2 - 150, startY + i * spacing - 5,
                300, 
                20,
                zolty, 
                niebieski
            );
        }
        
        DrawString(screen, SCREEN_WIDTH / 2 - strlen(options[i])*8/2, startY + i * spacing, 
            options[i], 
            charset
        );
    }
    
    // instrukcja
    char info[] = "Strzalki - wybor, Enter - potwierdzenie";
    DrawString(screen, SCREEN_WIDTH / 2 - strlen(info)*8/2, 400, info, charset);
}

int menuRun(SDL_Surface* screen, SDL_Surface* charset, SDL_Renderer* renderer, SDL_Texture* scrtex,
            int czerwony,int  niebieski,int zielony,int czarny) {
    MenuOption selected = MENU_START_GAME;
    int quit = 0;
    int result = 0; // 0 = wyjscie, 1 = start gry
    
    SDL_Event event;
    
    while (!quit) {
        menuDraw(screen, charset, selected, czerwony, niebieski, zielony, czarny);
        
        // wyswietlanie
        SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);
        SDL_RenderCopy(renderer, scrtex, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // obsluga eventow
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
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
                            quit = 1;
                            result = 1; // start gry
                        } else if (selected == MENU_HIGHSCORES) {
                            // TODO: wyswietl wyniki
                            printf("Wyniki - jeszcze nie zaimplementowane\n");
                        } else if (selected == MENU_EXIT) {
                            quit = 1;
                            result = 0;
                        }
                        break;
                    
                    case SDLK_ESCAPE:
                        quit = 1;
                        result = 0;
                        break;
                }
            }
        }
        
        SDL_Delay(16);
    }
    
    return result;
}