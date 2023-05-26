#pragma once
#include "chip8.h"
#include <iostream>
#include <SDL.h>
#include <vector>

class sdl_config {
	private:
		//SDL Related
		SDL_Window* window;
		SDL_Renderer* renderer;

		//Emulator Settings Related
		uint32_t window_height;
		uint32_t window_width;
		uint32_t scale_factor;
		uint32_t fg_color;
		uint32_t bg_color;
	public:
		sdl_config(uint32_t wh, uint32_t ww, uint32_t scale_factor, uint32_t fg_c, uint32_t bg_c);
		~sdl_config();
	
		void clear_display();
		void update_display(chip8* chip);
		std ::vector<uint32_t> get_window_dimen();
};