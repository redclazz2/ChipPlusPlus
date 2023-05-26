/*
					  Welcome to My Chip 8 Interpreter/Emulator!
	I've always wanted to dive into developing an emulator and multiple posts recommend
	to get your feet wet with the chip 8. Multiple routines are based of the ones present
	in video series: https://www.youtube.com/watch?v=EW1zXX89pfM&list=PLT7NbkyNWaqbyBMzdySdqjnfUFxt8rnU_&index=2,
	however, coming from a C# background I'm mostly familiar with OOP so I decided to practice
	it on C++. Please note this is my first project on C++ and some routines might be (weird)!
*/


#include "sdl_config.h"
#include "chip8.h"

int main(int argc, char* argv[]) {

	//Default Start
	if (argc < 2) {
		fprintf(stderr, "USSAGE: %s <rom_name>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	//Initialize SDL Configuration and SDL Components
	sdl_config sdl_conf{(uint32_t)32,(uint32_t)64,(uint32_t)20,(uint32_t)0xffffffff,(uint32_t)0x00000000 };

	//Intialize Chip8 Machine
	chip8 chip{argv[1]};

	//Clear Screen
	sdl_conf.clear_display();

	//Enter Emulator Loop
	while (chip.getEmulatorState() != chip8::QUIT) {
		//Get Input
		chip.handle_input();
		
		//If Emulator Paused, Continue
		if (chip.getEmulatorState() == chip8::PAUSED) continue;
		
		//Instruction Handling
		chip.handle_instructions(sdl_conf.get_window_dimen());

		//Delay
		SDL_Delay(16);

		//Update Screen
		sdl_conf.update_display(&chip);
	}
	printf("Bye!\n");
	return 0;
}