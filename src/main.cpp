#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <csignal>
#include <iostream>
#include "midi.h"
#include "button.h"
#include "slider.h"
#include "nav.h"
#include "config.h"
#include "events.h"

volatile sig_atomic_t quit_flag = 0;
void handle_sigint(int sig) { quit_flag = 1; }

constexpr int FRAME_DELAY_MS = 1000/60;

int main() {
    std::signal(SIGINT, handle_sigint);

    if(!init_midi()){ std::cerr << "Cannot initialize MIDI\n"; return 1; }

    if(SDL_Init(SDL_INIT_VIDEO) != 0){ std::cerr << "SDL_Init error\n"; return 1; }
    if(TTF_Init() != 0){ std::cerr << "TTF_Init error\n"; return 1; }

    SDL_Window* window = SDL_CreateWindow("SDL Pedal",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){ std::cerr << "SDL_CreateWindow failed\n"; return 1; }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){ std::cerr << "SDL_CreateRenderer failed\n"; return 1; }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    if(!font){ std::cerr << "TTF_OpenFont failed\n"; return 1; }

    // Load all controls (buttons + sliders) from config file
    std::vector<Control> controls = load_controls("buttons.txt");

    // Initialize navigation buttons
    init_nav_buttons();

    bool dirty = true;
    Uint32 last_frame = SDL_GetTicks();

    while(!quit_flag){
	    process_events(controls, quit_flag, dirty, current_page);
 // Draw only if dirty or frame elapsed
        Uint32 now = SDL_GetTicks();
        if(dirty || (now - last_frame >= FRAME_DELAY_MS)){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Draw all controls for current page
            for(auto &c: controls) draw_control(renderer, font, c, current_page);

            // Draw navigation buttons
            draw_nav_buttons(renderer, font);

            SDL_RenderPresent(renderer);

            dirty = false;
            last_frame = now;
        }

        SDL_Delay(1);
    }

    // Cleanup
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    close_midi();

    std::cout << "Exited cleanly.\n";
    return 0;
}

