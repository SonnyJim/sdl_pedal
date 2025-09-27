#include "button.h"
#include "midi.h"
#include "config.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>   // std::system
#include <thread>    // std::thread

// --- helper functions for buttons ---
static bool handle_single_button(Button& b, int current_page, SDL_Event &e){
    bool dirty=false;
    if(e.type==SDL_MOUSEBUTTONDOWN){
        int mx=e.button.x,my=e.button.y;
        if(b.page==current_page &&
           mx>=b.rect.x && mx<b.rect.x+b.rect.w &&
           my>=b.rect.y && my<b.rect.y+b.rect.h){

            if(b.type=="toggle"){
                b.state=!b.state;
                send_cc(b.cc,b.state?127:0);
                dirty=true;
            }
            else if(b.type=="cmd"){
                if(!b.cmd.empty()){
                    // run asynchronously to avoid freezing UI
                    std::thread([cmd=b.cmd](){
                        std::system(cmd.c_str());
                    }).detach();
                    dirty=true;
                } else {
                    //send_cmd(b.cc);
		    //TODO Print an error or something
                    dirty=true;
                }
            }
            else if(!b.pressed){
                b.pressed=true;
                send_cc(b.cc,127);
                dirty=true;
            }
        }
    } else if(e.type==SDL_MOUSEBUTTONUP){
        if(b.page==current_page && b.type=="button" && b.pressed){
            b.pressed=false;
            send_cc(b.cc,0);
            dirty=true;
        }
    }
    return dirty;
}

static void draw_single_button(SDL_Renderer* renderer, TTF_Font* font, const Button& b, int current_page){
    if(b.page!=current_page) return;

    // color based on type/state
    if(b.type=="toggle"){
        SDL_SetRenderDrawColor(renderer, b.state?0:200, b.state?200:0, 0, 255);
    } else if(b.type=="cmd"){
        SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255); // blue for cmd
    } else {
        SDL_SetRenderDrawColor(renderer, b.pressed?0:200, b.pressed?200:0, 0, 255);
    }
    SDL_RenderFillRect(renderer,&b.rect);

    SDL_Color white={255,255,255};
    SDL_Surface* surf=TTF_RenderText_Solid(font,b.label.c_str(),white);
    SDL_Texture* tex=SDL_CreateTextureFromSurface(renderer,surf);
    SDL_Rect dst={b.rect.x+10,b.rect.y+10,surf->w,surf->h};
    SDL_RenderCopy(renderer,tex,NULL,&dst);
    SDL_FreeSurface(surf); SDL_DestroyTexture(tex);
}

// --- unified control functions ---
bool handle_control_event(Control& c, int current_page, SDL_Event &e){
    if(c.is_slider) return handle_slider_event(c.slider,current_page,e);
    else return handle_single_button(c.button,current_page,e);
}

void draw_control(SDL_Renderer* renderer, TTF_Font* font, const Control& c, int current_page){
    if(c.is_slider) draw_slider(renderer, c.slider, current_page, font, c.button.label);
    else draw_single_button(renderer,font,c.button,current_page);
}

// --- load_controls ---
std::vector<Control> load_controls(const std::string& filename) {
    std::vector<Control> controls;
    std::ifstream file(filename);
    if(!file.is_open()){ std::cerr << "Cannot open " << filename << std::endl; exit(1); }

    int auto_x = 10, auto_y = 10; // auto-layout
    std::string line;
    while(std::getline(file,line)){
        if(line.empty() || line[0]=='#') continue;

        std::string label;
        int cc=-1;
        int page=0;
        std::string type="button";
        bool toggle=false;
        int x=-1, y=-1;
        std::string cmd;

        std::stringstream ss(line);
        std::string token;
        while(std::getline(ss, token, ',')){
            auto pos = token.find('=');
            if(pos==std::string::npos) continue;
            std::string key = token.substr(0,pos);
            std::string val = token.substr(pos+1);
            if(key=="label") label=val;
            else if(key=="cc") cc=std::stoi(val);
            else if(key=="type") type=val;
            else if(key=="page") page=std::stoi(val);
            else if(key=="x") x=std::stoi(val);
            else if(key=="y") y=std::stoi(val);
            else if(key=="cmd") cmd=val;
        }

        if(cc==-1 && type!="cmd"){ std::cerr<<"Missing CC in line: "<<line<<std::endl; exit(1);}
        if(label.empty()) label="CC"+std::to_string(cc);
        if(type=="toggle") toggle=true;

        Control c;
        int px = (x>=0)? x : auto_x;
        int py = (y>=0)? y : auto_y;

        if(type=="slider"){
            c.is_slider = true;
            c.slider = { {px,py,200,20}, {px,py,10,20}, cc, 0, false, page };
            c.button.label = label;
        } else {
            c.is_slider = false;
            c.button = { {px,py,80,40}, label, cc, false, toggle, false, page, false, type, cmd };
        }

        controls.push_back(c);

        if(x<0){ // only advance auto layout if user didn't specify coordinates
            auto_x += (c.is_slider?220:90);
            if(auto_x + 90 > SCREEN_WIDTH){
                auto_x = 10;
                auto_y += 50;
            }
        }
    }

    return controls;
}

