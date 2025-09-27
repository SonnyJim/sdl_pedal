#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "button.h"
#include "slider.h"
#include "nav.h"
#include <csignal> // for sig_atomic_t

bool process_events(std::vector<Control>& controls,
                    volatile sig_atomic_t& quit_flag,
                    bool& dirty,
                    int& current_page);

