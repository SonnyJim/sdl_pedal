#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "slider.h"

struct Button {
    SDL_Rect rect;
    std::string label;
    int cc, pc;
    bool pressed;
    bool toggle;    // toggle button
    bool state;     // current toggle state
    int page;
    bool is_slider; // for consistency
    std::string type; // "button", "toggle", "cmd"
    std::string cmd;  // shell command (used if type=="cmd")
};

struct Control {
    bool is_slider;
    Button button;
    Slider slider;
};

std::vector<Control> load_controls(const std::string& filename);
bool handle_control_event(Control& c, int current_page, SDL_Event &e);
void draw_control(SDL_Renderer* renderer, TTF_Font* font, const Control& c, int current_page);

// placeholder MIDI functions
//void send_cc(int cc, int val);
//void send_cmd(int cc); // fallback if cmd not set

