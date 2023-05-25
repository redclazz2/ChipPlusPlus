#pragma once
#include <iostream>
#include <SDL.h>
#include <stack>

class chip8{
	public:
		//Emulator Status
		typedef enum emulator_state {
			QUIT,
			RUNNING,
			PAUSED
		};

	private:
		emulator_state state;		//Emulator Status, Based on ENUM
		uint8_t ram[4096];			//4096 bytes of ram
		uint8_t display[64 * 32];	//Original resolution of display
		uint16_t PC;				//Program Counter, Points at current instruction in memory
		uint16_t I;					//I Register, Points at a location in memory
		uint8_t registers[16];		//Registers V0-VF
		std::stack<uint16_t> stack; //Subroutine Stack
		uint8_t delay_timer;		//Delay Timer. Decreases at 60Hz until 0. Stops Instructions if>0
		uint8_t sound_timer;		//Sound Timer. Decreases at 60Hz until 0. Plays Sound if >0
		const char* rom_name;		//Current ROM
		bool keypad[16];			//Emulation of Keyboard

	public:
		chip8(const char rom[]);
		~chip8();

		emulator_state getEmulatorState();
		void handle_input();
};

