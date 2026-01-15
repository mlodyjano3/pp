
#include <SDL.h>
#include <SDL_main.h>

int LoadFiles(SDL_Surface** screen, SDL_Surface** charset, SDL_Surface** eti,
               SDL_Window* window, SDL_Renderer* renderer, SDL_Texture* scrtex, SDL_Surface** sprite) {
    *charset = SDL_LoadBMP("./assets/bmps/cs8x8.bmp");
	if (*charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(*screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};
	*eti = SDL_LoadBMP("./assets/bmps/eti.bmp");
	if (*eti == NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(*charset);
		SDL_FreeSurface(*screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	};

	*sprite = SDL_LoadBMP("./assets/bmps/sprite.bmp");
	if (*sprite == NULL) {
		printf("SDL_LoadBMP(eti.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(*charset);
		SDL_FreeSurface(*screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
	}
	SDL_SetColorKey(*sprite, SDL_TRUE, SDL_MapRGB((*sprite)->format, 255, 0, 255));
    return 0;
}