#include "events.h"

bool process_events(std::vector<Control>& controls,
                    volatile sig_atomic_t& quit_flag,
                    bool& dirty,
                    int& current_page) {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            quit_flag = 1;
            return true;
        }

        // Navigation buttons
        if(e.type == SDL_MOUSEBUTTONDOWN) {
            int mx = e.button.x, my = e.button.y;
            for(int i = 0; i < nav_buttons.size(); ++i) {
                auto &b = nav_buttons[i];
                if(mx >= b.rect.x && mx < b.rect.x+b.rect.w &&
                   my >= b.rect.y && my < b.rect.y+b.rect.h) {
                    if(current_page != i) {
                        current_page = i;
                        dirty = true;
                    }
                }
            }
        }

        // Controls
        for(auto &c: controls)
            dirty |= handle_control_event(c, current_page, e);
    }
    return dirty;
}

