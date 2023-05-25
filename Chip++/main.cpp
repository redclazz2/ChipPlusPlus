#include <iostream>
#include <SDL.h>
#include "sdl_config.h"

int main(int argc, char* argv[]) {

	//Default
	//if (argc < 2) {
	//	fprintf(stderr, "USSAGE: %s <rom_name>\n", argv[0]);
	//	exit(EXIT_FAILURE);
	//}

	//Initialize SDL Configuration and SDL Components
	sdl_config sdl_conf{(uint32_t)32,(uint32_t)64,(uint32_t)20,(uint32_t)0xffffffff,(uint32_t)0x00000000 };

	//Intialize Chip8 Machine

	//Clear Screen
	sdl_conf.clear_display();

	//Enter Emulator Loop
	while (true) {
		//Get Input

		//If Emulator Paused, Continue

		//Instruction Handling

		//Delay
		SDL_Delay(16.6);

		//Update Screen
		sdl_conf.update_display();
	}

	sdl_conf.~sdl_config();
	return 0;
}