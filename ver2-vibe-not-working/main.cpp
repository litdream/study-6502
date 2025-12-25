#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <thread>
#include <chrono>
#include "memory.hpp"
#include "cpu.hpp"

// Screen dimensions
const int SCREEN_WIDTH = 560; // 40 columns * 14 pixels/char (approx)
const int SCREEN_HEIGHT = 384; // 24 rows * 16 pixels/char
const int APPLE_CHAR_WIDTH = 14;
const int APPLE_CHAR_HEIGHT = 16;

// Apple II memory addresses
const uint16_t TEXT_SCREEN_START = 0x0400;
const uint16_t TEXT_SCREEN_END = 0x07FF;
const uint16_t KEYBOARD_STROBE = 0xC000;
const uint16_t KEYBOARD_DATA = 0xC010;

// Function to map ASCII to Apple II keycodes (very basic for now)
uint8_t map_ascii_to_appleii(int key) {
    if (key >= 'a' && key <= 'z') {
        return (uint8_t)(key - 'a' + 0xC1); // A-Z start at C1
    } else if (key >= 'A' && key <= 'Z') {
        return (uint8_t)(key - 'A' + 0xC1); // A-Z start at C1
    } else if (key >= '0' && key <= '9') {
        return (uint8_t)(key - '0' + 0xB0); // 0-9 start at B0
    } else if (key == '\r') { // Return key
        return 0x8D;
    } else if (key == ' ') {
        return 0xA0;
    } else if (key == '.') {
        return 0xAE;
    } else if (key == ',') {
        return 0xAC;
    } else if (key == ':') {
        return 0xBA;
    } else if (key == ';') {
        return 0xBB;
    } else if (key == '(') {
        return 0xB8;
    }
    return 0x00; // Unhandled key
}

int main(int argc, char* args[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("Apple II+ Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("PrintChar21.ttf", APPLE_CHAR_HEIGHT);
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Setup CPU and Memory
    Memory memory;
    CPU cpu(memory);

    try {
        // Load the 6 individual 2KB ROM files for Applesoft BASIC and the Monitor ROM.
        memory.loadROM("ROM/Apple II plus ROM Pages D0-D7 - 341-0011 - Applesoft BASIC.bin", 0xD000);
        memory.loadROM("ROM/Apple II plus ROM Pages D8-DF - 341-0012 - Applesoft BASIC.bin", 0xD800);
        memory.loadROM("ROM/Apple II plus ROM Pages E0-E7 - 341-0013 - Applesoft BASIC.bin", 0xE000);
        memory.loadROM("ROM/Apple II plus ROM Pages E8-EF - 341-0014 - Applesoft BASIC.bin", 0xE800);
        memory.loadROM("ROM/Apple II plus ROM Pages F0-F7 - 341-0015 - Applesoft BASIC.bin", 0xF000);
        memory.loadROM("ROM/Apple II plus ROM Pages F8-FF - 341-0020 - Autostart Monitor.bin", 0xF800);
    } catch (const std::runtime_error& e) {
        std::cerr << "ROM loading error: " << e.what() << std::endl;
        // Cleanup before exiting
        TTF_CloseFont(font);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Set CPU to reset vector
    cpu.reset();

    bool quit = false;
    SDL_Event e;

    const int CYCLES_PER_FRAME = 17050; // Approx cycles for a 60Hz refresh rate

    std::vector<uint8_t> last_text_screen(TEXT_SCREEN_END - TEXT_SCREEN_START + 1);

    // Main emulation loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                int key = e.key.keysym.sym;
                uint8_t apple_key = map_ascii_to_appleii(key);
                if (apple_key != 0x00) {
                    memory.keyPress(apple_key);
                }
            }
        }

        // Emulate CPU cycles for one frame
        // cpu.emulateCycles(CYCLES_PER_FRAME);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);

        // Render text from Apple II memory
        SDL_Color textColor = {0xFF, 0xFF, 0xFF, 0xFF}; // White text
        int char_index = 0;
        for (int row = 0; row < 24; ++row) {
            for (int col = 0; col < 40; ++col) {
                uint16_t memory_address = TEXT_SCREEN_START + (row & 0x7) * 0x80 + ((row >> 3) & 0x3) * 0x28 + col;
                uint8_t apple_char_code = memory.read(memory_address);

                // Only redraw if the character has changed
                if (apple_char_code != last_text_screen[char_index]) {
                    char display_char = ' ';
                    // Apple II Character Set Mapping
                    if (apple_char_code >= 0x20 && apple_char_code <= 0x5F) { // Standard characters (inverse)
                        display_char = (char)(apple_char_code + 0x40);
                    } else if (apple_char_code >= 0x40 && apple_char_code <= 0x7F) { // Standard characters
                        display_char = (char)(apple_char_code);
                    } else if (apple_char_code >= 0x80 && apple_char_code <= 0xBF) { // Flashing characters (inverse)
                        display_char = (char)(apple_char_code - 0x80 + 0x40);
                    } else if (apple_char_code >= 0xC0 && apple_char_code <= 0xFF) { // Flashing characters
                        display_char = (char)(apple_char_code - 0xC0);
                    } else if (apple_char_code == 0x0A) { // Newline character (control code)
                        display_char = '\n'; // Represent as newline, but we skip rendering it
                    }

                    std::string s(1, display_char);
                    if (display_char != '\n') { // Skip rendering for newline
                        SDL_Surface* textSurface = TTF_RenderText_Solid(font, s.c_str(), textColor);
                        if (textSurface == nullptr) {
                            std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
                        } else {
                            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                            if (textTexture == nullptr) {
                                std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
                            } else {
                                int x = col * APPLE_CHAR_WIDTH;
                                int y = row * APPLE_CHAR_HEIGHT;
                                SDL_Rect renderQuad = {x, y, APPLE_CHAR_WIDTH, APPLE_CHAR_HEIGHT};
                                SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
                                SDL_DestroyTexture(textTexture);
                            }
                            SDL_FreeSurface(textSurface);
                        }
                    }
                }
                last_text_screen[char_index] = apple_char_code;
                char_index++;
            }
        }

        // Update screen
        SDL_RenderPresent(renderer);

        // Introduce a small delay to control emulation speed and responsiveness
        SDL_Delay(100);
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    std::cout << "Main loop exited." << std::endl;
    return 0;
}
