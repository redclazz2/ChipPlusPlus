#include "chip8.h"

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

        //FROM: https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#7xnn-add
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
                    printf("----PAUSED----\n");
                }
                else {
                    state = RUNNING;
                    printf("----RESUMED----\n");
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

/// <summary>
/// Handles instructions based on OPCODES for chip8
/// </summary>
void chip8::handle_instructions(std::vector<uint32_t> dimensions){
    
    /*
        | lets store two 8bit numbers in 16bits
        Since Chip8 instructions are read from 2 bytes
        first you read the PC pointer shifted from left 1 byte then the next direction (+1)
        Finally increase PC + 2 for next iteration
    */

    uint16_t instruction = (this->ram[this->PC] << 8) | this->ram[this->PC + 1];
    this->PC += 2;
    printf("Address: 0x%04X, Opcode: 0x%04X\n",this->PC-2, instruction);

    /*Opcodes are formatted in OxO(f)O(s)O(rd)O(th) format
      This means we have 4 nibbles, offsets are:

      first: >> 12 mover desde la derecha 12 bits
      second: >> 8 mover desde la derecha 8 bits
      third: >> 4 mover desde la derecha 4 bits

    */

    uint8_t X = (instruction >> 8) & 0x0F;
    uint8_t Y = (instruction >> 4) & 0x0F;
    uint8_t N = instruction & 0x0F; //Gets 4th nibble
    uint8_t NN = instruction & 0x0FF; //Gets 1.5 nibbles (1 Bytes)
    uint16_t NNN = instruction & 0x0FFF; //Gets 3 nibbles (1.5 Bytes)

    /* If we want to access first nibble(how chip8 handles opcodes) shift by 12 
       bitwise-it to remove un. bits.
    */
    switch ((instruction >> 12) & 0x0F) {
        case 0x00:

            switch (NN) {
                case 0xE0:
                    printf("R:Clear Screen.\n");
                    memset(&display[0], false, sizeof this->display);
                    break;

                case 0xEE:
                    printf("R:Return from subroutine.\n");
                    this->PC = stack.top();
                    stack.pop();
                    break;

                default:
                    printf("E:Unimplemented instruction 0x%04X\n", instruction);
                    break;
            }

            break;

        case 0x1:
            printf("R:Jump from: 0x%04X to: 0x%04X\n",this->PC,NNN);
            this->PC = NNN;
            break;

        case 0x02:
            printf("R:Saving subroutine to stack.\n");
            stack.push(this->PC);
            this->PC = NNN;
            break;

        case 0x03:
            printf("R:Skipping if VX == NN.\n");
            if (this->registers[X] == NN) this->PC += 2;
            break;

        case 0x06:
            printf("R:V[%X] Set to 0x%04X\n",X,NN);
            this->registers[X] = NN;
            break;

        case 0x07:
            printf("R:Add NN to V[x]\n");
            this->registers[X] += NN;
            break;

        case 0x0A:
            printf("R:Set I to NNN\n");
            this->I = NNN;
            break;

        case 0x0D:
        {
            /*Draws a sprite to screen and sets VF to true, signaling a collision event
              To draw a sprite you start at the top left of the screen.*/

            printf("R:Draw to Display\n");
            std::vector<uint32_t> dimen = dimensions;
            //std::cout << dimen[0] << " " << dimen[1];

            uint8_t X_coord = this->registers[X] % dimen[0];
            uint8_t Y_coord = this->registers[Y] % dimen[1];
            const uint8_t orig_x = X_coord; //Original X Value

            this->registers[0xF] = 0; //Initialize carry flag to 0

            //Loops all rows of sprites
            for (uint8_t i = 0; i < N; i++) {
                const uint8_t sprite_data = this->ram[I + i];
                X_coord = orig_x; //Reset x for next row draw

                for (int8_t j = 7; j >= 0; j--) {
                    bool* pixel = &display[Y_coord * dimen[0] + X_coord];
                    bool sprite_bit = (sprite_data & (1 << j));

                    //If sprite pixel is on and display pixel is on set carry flag
                    if (sprite_bit && *pixel) {
                        this->registers[0xF] = 1;
                    }

                    // Exclusive OR to set pixel on or off
                    *pixel ^= sprite_bit;

                    if (++X_coord >= dimen[0]) break; //Stop if X of sprite is greater than width
                }

                if (++Y_coord >= dimen[1]) break; //Stop if Y of sprite is greater than height
            }
            break;
        }

        default:
            printf("E:Unimplemented instruction 0x%04X\n", instruction);
            break;
    }
}
