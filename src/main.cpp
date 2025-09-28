#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <csignal>
#include <iostream>
#include <vector>
#include <string>
#include "midi.h"
#include "button.h"
#include "slider.h"
#include "nav.h"
#include "config.h"
#include "events.h"

volatile sig_atomic_t quit_flag = 0;
void handle_sigint(int sig) { quit_flag = 1; }

constexpr int FRAME_DELAY_MS = 1000/60;

bool verbose = false; // global verbose flag

int main(int argc, char* argv[]) {
    // Parse command line arguments
    for(int i = 1; i < argc; i++) {
        std::string arg(argv[i]);
        if(arg == "-v" || arg == "--verbose") {
            verbose = true;
            std::cout << "Verbose mode enabled\n";
        }
    }

    std::signal(SIGINT, handle_sigint);

    if(!init_midi()){ 
        std::cerr << "Cannot initialize MIDI\n"; 
        return 1; 
    }

    if(SDL_Init(SDL_INIT_VIDEO) != 0){ 
        std::cerr << "SDL_Init error: " << SDL_GetError() << "\n"; 
        return 1; 
    }
    if(TTF_Init() != 0){ 
        std::cerr << "TTF_Init error: " << TTF_GetError() << "\n"; 
        return 1; 
    }

    SDL_Window* window = SDL_CreateWindow("SDL Pedal",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){ 
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n"; 
        return 1; 
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){ 
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n"; 
        return 1; 
    }

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 16);
    if(!font){ 
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n"; 
        return 1; 
    }

    if(verbose) std::cout << "Loading controls...\n";
    std::vector<Control> controls = load_controls("buttons.txt");

    if(verbose) std::cout << "Initializing navigation buttons...\n";
    init_nav_buttons();

    bool dirty = true;
    Uint32 last_frame = SDL_GetTicks();

    if(verbose) std::cout << "Entering main loop...\n";

    while(!quit_flag){
        process_events(controls, quit_flag, dirty, current_page);

        Uint32 now = SDL_GetTicks();
        if(dirty || (now - last_frame >= FRAME_DELAY_MS)){
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for(auto &c: controls) draw_control(renderer, font, c, current_page);
            draw_nav_buttons(renderer, font);

            SDL_RenderPresent(renderer);

            //if(verbose) std::cout << "Frame drawn at " << now << "ms\n";

            dirty = false;
            last_frame = now;
        }

        SDL_Delay(1);
    }

    if(verbose) std::cout << "Cleaning up...\n";

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    close_midi();

    std::cout << "Exited cleanly.\n";
    return 0;
}

