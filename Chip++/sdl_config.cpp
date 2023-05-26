#include "sdl_config.h"

#pragma region Constructor & Destructor
	
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
		printf("Disposing SDL Objects\n");
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
	void sdl_config::update_display(chip8* chip) {

		SDL_Rect rect = {0,0,this->scale_factor,this->scale_factor};

		std::vector<uint8_t> bcolor = sdl_config_get_rgba(&bg_color);
		std::vector<uint8_t> fcolor = sdl_config_get_rgba(&fg_color);

		//Loop through display pixels, draw a rectangle per pixel to the SDL Window
		for (uint32_t i = 0; i < sizeof chip->display; i++) {
			rect.x = (i % this->window_width) * this->scale_factor;
			rect.y = (i / this->window_width) * this->scale_factor;

			if (chip->display[i]) {
				//Pixel is on draw foreground
				SDL_SetRenderDrawColor(this->renderer, fcolor[0], fcolor[1], fcolor[2], fcolor[3]);
				SDL_RenderFillRect(this->renderer, &rect);

				//Adds pixelated effect
				SDL_SetRenderDrawColor(this->renderer, bcolor[0], bcolor[1], bcolor[2], bcolor[3]);
				SDL_RenderDrawRect(this->renderer, &rect);
			}
			else {
				//Pixel is Off
				SDL_SetRenderDrawColor(this->renderer, bcolor[0], bcolor[1], bcolor[2], bcolor[3]);
				SDL_RenderFillRect(this->renderer, &rect);
			}
		}

		SDL_RenderPresent(this->renderer);
	}
	/// <summary>
	/// Returns a vector with the width and height of the window
	/// </summary>
	/// <returns></returns>
	std::vector<uint32_t> sdl_config::get_window_dimen() {
		return std::vector<uint32_t> {window_width, window_height};
	}
#pragma endregion
