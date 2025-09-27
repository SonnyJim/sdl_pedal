#include "slider.h"
#include "midi.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

bool handle_slider_event(Slider &s, int current_page, SDL_Event &e){
    if(s.page != current_page) return false;

    bool dirty = false;
    if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION){
        int mx = (e.type==SDL_MOUSEMOTION)? e.motion.x : e.button.x;
        int my = (e.type==SDL_MOUSEMOTION)? e.motion.y : e.button.y;

        bool left_pressed = (e.type==SDL_MOUSEBUTTONDOWN)? true : (e.motion.state & SDL_BUTTON(SDL_BUTTON_LEFT));
        if(!left_pressed) return false;

        if(mx >= s.track.x && mx <= s.track.x + s.track.w){
            int old_value = s.value;
            float ratio = float(mx - s.track.x) / float(s.track.w);
            s.value = int(ratio * 127);
            if(s.value != old_value){
                dirty = true;
                send_cc(s.cc, s.value);
                std::cout << "Slider CC=" << s.cc << " value=" << s.value << " (mx=" << mx << ")\n";
            }
        }
    }
    return dirty;
}

void draw_slider(SDL_Renderer* renderer, const Slider &s, int current_page, TTF_Font* font, const std::string &label){
    if(s.page != current_page) return;

    // Draw track
    SDL_SetRenderDrawColor(renderer, 100,100,100,255);
    SDL_RenderFillRect(renderer, &s.track);

    // Draw knob (compute knob x locally)
    int knob_x = s.track.x + int((s.value/127.0f)*(s.track.w - s.knob.w));
    SDL_Rect knob_rect = s.knob;
    knob_rect.x = knob_x;
    SDL_SetRenderDrawColor(renderer, 200,0,0,255);
    SDL_RenderFillRect(renderer, &knob_rect);

    // Draw label to the left of slider
    if(font){
        SDL_Color white = {255,255,255};
        SDL_Surface* surf = TTF_RenderText_Solid(font, label.c_str(), white);
        if(surf){
            SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
            // Position label to the left of the slider
            SDL_Rect dst = { s.track.x - surf->w - 5, s.track.y + (s.track.h - surf->h)/2, surf->w, surf->h };
            SDL_RenderCopy(renderer, tex, NULL, &dst);
            SDL_FreeSurface(surf);
            SDL_DestroyTexture(tex);
        }
    }
}

