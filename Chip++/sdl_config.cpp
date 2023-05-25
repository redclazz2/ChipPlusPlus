#include "sdl_config.h"
#include <iostream>
#include <SDL.h>
#include <vector>

#pragma region Constructor and Destructor
	
	/// <summary>
	/// sdl_config class constructor
	/// </summary>
	/// <param name="wh"></param>
	/// <param name="ww"></param>
	/// <param name="scale_factor"></param>
	/// <param name="fg_c"></param>
	/// <param name="bg_c"></param>
	sdl_config::sdl_config(uint32_t wh, uint32_t ww, uint32_t scale_factor, uint32_t fg_c, uint32_t bg_c) {

		printf("Initializing SDL and EMULATOR SETTINGS...\n");

		//Initialize SDL Components
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
			printf("An Error has ocurred with SDL. -INIT ERROR-\n");
			exit(EXIT_FAILURE);
		}

		//Assign values passed on constructor to object
		this->window_height = wh;
		this->window_width = ww;
		this->scale_factor = scale_factor;
		this->fg_color = fg_c;
		this->bg_color = bg_c;

		this->window = SDL_CreateWindow("ChipPlusPlus", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			window_width * this->scale_factor, window_height * this->scale_factor, 0);
		if (window == NULL) {
			printf("An Error has occurred with SDL WINDOW. -WINDOW ERROR-\n");
			exit(EXIT_FAILURE);
		}

		this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL) {
			printf("An Error has occurred with SDL RENDERER. -RENDERER ERROR-\n");
			exit(EXIT_FAILURE);
		}

		printf("SDL and EMULATOR SETTINGS setup completed!\n");
	}

	/// <summary>
	/// Properly destroys SDL Objects and stops SDL components
	/// </summary>
	sdl_config::~sdl_config() {
		printf("Disposing SDL Objects");
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
	}

#pragma endregion

#pragma region Methods
	/// <summary>
	/// Returns a vector with the proper color hexs in RGBA form
	/// </summary>
	/// <param name="hexcolor"></param>
	/// <returns></returns>
	std::vector<uint8_t> sdl_config_get_rgba(uint32_t* hexcolor) {
		std::vector<uint8_t> color = {0,0,0,0};
		uint8_t offset = 24;

		for (int i = 0; i < 4; i++) {
			color[i] = *hexcolor >> offset & 0xFF;
			offset -= 8;
		}

		return color;
	}

	/// <summary>
	/// Sets the screen to a color (Background)
	/// </summary>
	void sdl_config::clear_display() {
		std::vector<uint8_t> color = sdl_config_get_rgba(&bg_color);

		SDL_SetRenderDrawColor(renderer,color[0],color[1],color[2],color[3]);
		SDL_RenderClear(renderer);
	}
	/// <summary>
	/// Updates Display Data Based On Chip8's Display, then presents the renderer with SDL.
	/// </summary>
	void sdl_config::update_display() {
		SDL_RenderPresent(this->renderer);
	}
#pragma endregion
