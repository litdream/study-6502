#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <cctype>
#include "memory.hpp"
#include "cpu.hpp"

// Screen dimensions
const int SCREEN_WIDTH = 560; // Apple II text screen is 40 characters * 14 pixels, let's use a larger window
const int SCREEN_HEIGHT = 384; // 24 lines * 16 pixels
const int FONT_SIZE = 16;

// CPU speed and target frame rate
const uint32_t CPU_CLOCK_HZ = 1023000; // Apple II 6502 clock speed
const uint32_t TARGET_FPS = 60;
const uint32_t CYCLES_PER_FRAME = CPU_CLOCK_HZ / TARGET_FPS;

void renderScreen(SDL_Renderer* renderer, TTF_Font* font, Memory& mem, SDL_Color& textColor) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int y = 0; y < 24; ++y) {
        for (int x = 0; x < 40; ++x) {
            uint16_t mem_addr = 0x0400 + y * 40 + x;
            uint8_t char_code = mem.read(mem_addr) & 0x7F; // Mask out high bit for ASCII
            
            if (char_code != 0) {
                char text[2] = { (char)char_code, '\0' };
                SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
                if (textSurface != nullptr) {
                    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                    if (textTexture != nullptr) {
                        SDL_Rect renderQuad = {x * FONT_SIZE, y * FONT_SIZE, textSurface->w, textSurface->h};
                        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
                        SDL_DestroyTexture(textTexture);
                    }
                    SDL_FreeSurface(textSurface);
                }
            }
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Apple II+ Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("PrintChar21.ttf", FONT_SIZE);
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    Memory mem;
    if (!mem.loadROM("Apple2_Plus.rom", 0xD000)) {
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    CPU cpu(mem);
    cpu.reset();
    uint8_t lowByte = mem.read(0xFFFC);
    uint8_t highByte = mem.read(0xFFFD);
    cpu.PC = (highByte << 8) | lowByte;

    bool quit = false;
    SDL_Event e;
    SDL_Color textColor = {255, 255, 255, 255};

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                SDL_Keycode keycode = e.key.keysym.sym;
                if (keycode >= 'a' && keycode <= 'z') {
                    keycode = toupper(keycode);
                }
                if (keycode == SDLK_RETURN) {
                    keycode = 0x0D;
                }
                mem.keyPress(static_cast<uint8_t>(keycode));
            }
        }

        cpu.emulateCycles(CYCLES_PER_FRAME);

        SDL_PumpEvents();
        
        renderScreen(renderer, font, mem, textColor);
        SDL_Delay(16); // Aim for ~60 FPS
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
