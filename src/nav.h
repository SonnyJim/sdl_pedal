#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "button.h"

extern std::vector<Button> nav_buttons;
extern int current_page;

void init_nav_buttons();
void draw_nav_buttons(SDL_Renderer* renderer, TTF_Font* font);

