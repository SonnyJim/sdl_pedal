#include "config.h"
#include "nav.h"
#include <string>

std::vector<Button> nav_buttons;
int current_page=0;

void init_nav_buttons() {
    int width=50, height=30, y=SCREEN_HEIGHT-height;
    for(int i=0;i<8;i++){
        Button b; b.rect={i*(width+5)+5, y, width, height};
        b.label="Pg"+std::to_string(i+1);
        b.cc=-1; b.toggle=false; b.pressed=false; b.state=false;
        nav_buttons.push_back(b);
    }
}

void draw_nav_buttons(SDL_Renderer* renderer, TTF_Font* font){
    for(int i=0;i<nav_buttons.size();i++){
        auto &b = nav_buttons[i];
        if(i==current_page) SDL_SetRenderDrawColor(renderer,0,200,0,255); // highlight
        else SDL_SetRenderDrawColor(renderer,0,0,150,255);
        SDL_RenderFillRect(renderer,&b.rect);
        SDL_Color white={255,255,255};
        SDL_Surface* surf=TTF_RenderText_Solid(font,b.label.c_str(),white);
        SDL_Texture* tex=SDL_CreateTextureFromSurface(renderer,surf);
        SDL_Rect dst={b.rect.x+5,b.rect.y+5,surf->w,surf->h};
        SDL_RenderCopy(renderer,tex,NULL,&dst);
        SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
    }
}

