#include "chip8.h"

#include <iostream>
#include <SDL.h>
#include <vector>

#pragma region Constructor & Destructor
    //Constructor of chip8
    chip8::chip8(const char rom_name[]) {
        *ram = { 0 };
        *display = { 0 };
        PC = 0x200;
        I = 0;
        *registers = { 0 };
        delay_timer = 0;
        sound_timer = 0;
        *keypad = { 0 };
        state = RUNNING;
        this->rom_name = rom_name;

        const uint8_t font[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
        };

        //Copy Font to RAM
        memcpy(&ram[0], font, sizeof(font));

    #pragma warning(disable:4996)
        FILE* rom = fopen(rom_name, "rb");
        if (!rom) {
            SDL_Log("Rom File is invalid or doesnt exists");
            exit(EXIT_FAILURE);
        }
        else {
            fseek(rom, 0, SEEK_END);
            const size_t rom_size = ftell(rom);
            const size_t max_size = sizeof ram - 0x200;
            rewind(rom);

            if (rom_size > max_size) {
                SDL_Log("Rom Size is Too Big.");
                exit(EXIT_FAILURE);
            }

            if (fread(&ram[0x200], rom_size, 1, rom) != 1) {
                SDL_Log("Unable to load ROM data to Chip 8 RAM.");
                exit(EXIT_FAILURE);
            }
            fclose(rom);
            printf("Succesfully Loaded ROM in Memory. PC at 0x200\n");
        }
    }

    //Deconstructor of chip8
    chip8::~chip8() {
        printf("Disposing Chip8\n");
    }
#pragma endregion

/// <summary>
/// Returns Emulator State
/// </summary>
/// <returns></returns>
chip8::emulator_state chip8::getEmulatorState() {
	return this->state;
}

/// <summary>
/// Handles input from SDL events
/// </summary>
void chip8::handle_input() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {

        case SDL_QUIT:
            state = QUIT;
            break;

        case SDL_KEYDOWN:

            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                //Exit Window and End Program
                state = QUIT;
                break;

            case SDLK_SPACE:
                if (state == RUNNING) {
                    state = PAUSED;
                    printf("----PAUSED----");
                }
                else {
                    state = RUNNING;
                    printf("----RESUMED----");
                }
                break;
            }

            break;

        case SDL_KEYUP:
            break;

        default:
            break;
        }
    }
}
