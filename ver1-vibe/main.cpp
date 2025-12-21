#include <SDL.h>
#include <iostream>
#include <cctype>
#include "memory.hpp"
#include "cpu.hpp"

// Screen dimensions
const int SCREEN_WIDTH = 280;
const int SCREEN_HEIGHT = 192;

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Apple II+ Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Memory mem;
    if (!mem.loadROM("Apple2_Plus.rom", 0xD000)) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    CPU cpu(mem);
    cpu.reset();
    // Set PC to reset vector
    uint8_t lowByte = mem.read(0xFFFC);
    uint8_t highByte = mem.read(0xFFFD);
    cpu.PC = (highByte << 8) | lowByte;

    bool quit = false;
    SDL_Event e;

    // Main loop
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                // The Apple II only deals with uppercase letters
                SDL_Keycode keycode = e.key.keysym.sym;
                if (keycode >= 'a' && keycode <= 'z') {
                    keycode = toupper(keycode);
                }
                // Apple II Return key is ASCII 13 ($0D)
                if (keycode == SDLK_RETURN) {
                    keycode = 0x0D;
                }
                mem.keyPress(static_cast<uint8_t>(keycode));
            }
        }

        // Execute a step
        cpu.step();

        // For now, no video rendering. We'll add that next.
        // A small delay to not run the CPU at max speed.
        SDL_Delay(1); 
    }

    // Destroy window and quit SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}