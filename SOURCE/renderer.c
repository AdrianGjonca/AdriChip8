#include <SDL2/SDL.h>
#include "renderer.h"

#include <stdlib.h>
#include "globals.h"

SDL_Window * window;
SDL_Renderer * renderer;
SDL_Rect main_rect;

void renderer_init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		fprintf(stderr, "Could not initialize SDL2: %s\n", SDL_GetError());
		exit(1);
	}
	window = SDL_CreateWindow(
		"AdriCHIP8 - CHIP-8 emulator made by Adrian Gjonca",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		600,600,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if(renderer == NULL) {
		fprintf(stderr, "Could not initialize SDL2 renderer: %s\n", SDL_GetError());
	}
}

void renderer_loop(void (*onEvent)(SDL_Event), void (*onFrame)()) {
	int running = 1;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {  // poll until all events are handled!
			onEvent(event);
			if(event.type == SDL_QUIT) {
				running = 0;
			}
		}
		int height, width;
		SDL_GetWindowSize(window, &width, &height);
		main_rect.x = main_rect.y = 10;
		if(width < height * 2) {
			main_rect.w = width - 20;
			main_rect.h = main_rect.w / 2;
			main_rect.y += (height / 2) - (main_rect.h / 2);
		} else {
			main_rect.h = height - 20;
			main_rect.w = main_rect.h * 2;
			main_rect.x += (width / 2) - (main_rect.w / 2);
		}	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, NULL);

		SDL_Rect border = main_rect;
		border.x -= 4;
		border.y -= 4;
		border.w += 8;
		border.h += 8;
		SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
		SDL_RenderFillRect(renderer, &border);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderFillRect(renderer, &main_rect);

		onFrame();		

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();	
}

void blot_white(int x, int y) {
	SDL_Rect blot = {
		.x = x * (main_rect.w/DISP_W) + main_rect.x,
		.y = y * (main_rect.y/DISP_H) + main_rect.y,
		.w = (main_rect.w/DISP_W),
		.h = (main_rect.h/DISP_H)
	};
	SDL_SetRenderDrawColor(renderer, 200, 200, 180, 255);
	SDL_RenderFillRect(renderer, &blot);
}

void blot_black(int x, int y) {
	SDL_Rect blot = {
		.x = x * (main_rect.w/DISP_W) + main_rect.x,
		.y = y * (main_rect.y/DISP_H) + main_rect.y,
		.w = (main_rect.w/DISP_W),
		.h = (main_rect.h/DISP_H)
	};
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &blot);
}
