//  Let's use clang++
//    (Assuming on Ubuntu-like linux)
//
//  $ clang++ -I/usr/include/SDL2 -lSDL2 example.cpp
//  $ ./a.out

#include <iostream>
#include <SDL.h>

int main(int argc, char **argv) {
	SDL_Surface *surface = NULL;
	SDL_Window  *window = NULL;

	if ( SDL_Init( SDL_INIT_EVERYTHING ) <0 ) {
		std::cerr << "Error init SDL: " << SDL_GetError() << std::endl;
		return 1;
	}

	// Window
	window = SDL_CreateWindow( "Example", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640, 480, SDL_WINDOW_SHOWN );
	if ( !window ) {
		std::cerr << "Error creating window: " <<SDL_GetError() <<std::endl;
		return 2;
	}

	// Surface
	surface = SDL_GetWindowSurface( window );
	if ( !surface ) {
		std::cerr << "Error getting surface: " << SDL_GetError() <<std::endl;
		return 3;
	}

	// White rectangle
	SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 255,255,255));
	
	// Update
	SDL_UpdateWindowSurface( window );


	SDL_Event event;
	bool running = true;
	
	while (running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		SDL_Delay(10); 
	}

	// Wrap up.
	SDL_DestroyWindow( window );
	SDL_Quit();

	return 0;
}
