#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

struct Slider {
    SDL_Rect track;   // full track rectangle
    SDL_Rect knob;    // movable knob
    int cc;           // MIDI CC number
    int value;        // 0-127
    bool pressed;     // dragging state
    int page;         // which page this slider belongs to
};

// Event handler (needs non-const reference)
bool handle_slider_event(Slider &s, int current_page, SDL_Event &e);

// Drawing function with label
void draw_slider(SDL_Renderer* renderer, const Slider &s, int current_page, TTF_Font* font, const std::string &label);

