#include <iostream>
#include <SDL_ttf.h>
#include <SDL.h>
#include <string>
const int width = 1020,height = 900;
int main(int argc, char *argv[])
{
	int textW = 0;
	int textH = 0;

	TTF_Init();
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		//std::cout << "SDL could not initialise! SDL Error:" << SDL_GetError();
	};

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Color Black = { 0,0,0 };
	SDL_Color White = { 255,255,255 };
	SDL_Color Red = { 255, 0, 0 };
	SDL_Color Blue = { 0, 0, 255 };
	SDL_Color Green = { 0, 255, 0 };
	TTF_Font* font = TTF_OpenFont("Font.ttf", 20);
	TTF_Font* font2 = TTF_OpenFont("Font2.ttf", 11);
	
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
	
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, "RMS", Black);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, "EDF", Black);
	SDL_Surface* surfaceMessage3 = TTF_RenderText_Solid(font, "Time", Black);
	SDL_Surface* surfaceMessage4 = TTF_RenderText_Solid(font, "Tasks", Black);
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_Texture *texture2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
	SDL_Texture *texture3 = SDL_CreateTextureFromSurface(renderer, surfaceMessage3);
	SDL_Texture *texture4 = SDL_CreateTextureFromSurface(renderer, surfaceMessage4);
	
	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
	
	SDL_Rect text{ 250,150,textW,textH };
	SDL_Rect text2{ 250,600,textW,textH };
	SDL_Rect text3{ 400,600,textW,textH };
	SDL_Rect text4{ 70,150,textW,textH };

	if (NULL == window)
	{
		std::cout << "Not working" << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	
	
	SDL_Event windowEvent;
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(renderer, 50, 150, 50, 800);
	SDL_RenderDrawLine(renderer, 50, 450, 650, 450);
	SDL_RenderCopy(renderer, texture, NULL, &text);
	SDL_RenderCopy(renderer, texture2, NULL, &text2);
	SDL_RenderCopy(renderer, texture3, NULL, &text3);
	SDL_RenderCopy(renderer, texture4, NULL, &text4);
	SDL_RenderPresent(renderer);
	int x = 0;
	while (x < 280) {
		SDL_RenderDrawLine(renderer, 120 + x, 298, 120 + x, 302);
		x = x + 5;
	}
	x = 0;
	int TaskNo;
	TaskNo = 4;
	int t = 25;
	int s = 150;
	std::string y;

	while (x < TaskNo) {
		y = std::to_string(x);
		const char *ystring = y.c_str();
		SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font2, ystring, Black);
		SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		SDL_Rect text5{80,s = s+t,textW,textH };
		SDL_RenderCopy(renderer, texture, NULL, &text5);
		SDL_RenderPresent(renderer);
		x = x + 1;
	}

	SDL_RenderPresent(renderer);
	while (true)
	{
		if (SDL_PollEvent( &windowEvent))
		{
			if (SDL_QUIT == windowEvent.type)
			{
				break;
			}
		}
	}
	TTF_CloseFont(font);
	TTF_CloseFont(font2);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_Quit();
	SDL_Quit( );
	
	return EXIT_SUCCESS;
}		
