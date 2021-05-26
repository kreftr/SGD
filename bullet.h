#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

class Bullet{

public:

    coordinate position;
    float power_x = 3;
    float power_y = 3;
    double angle;

    Bullet(coordinate pos, double a){
        position.x = pos.x;
        position.y = pos.y;
        angle = a;
    }


    void draw(SDL_Renderer *renderer){
        position.x += cos(angle) * power_x;
        position.y += sin(angle) * power_y;

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(renderer,position.x,position.y);
        // SDL_RenderDrawPoint(renderer,position.x+1,position.y);
        // SDL_RenderDrawPoint(renderer,position.x-1,position.y);
        // SDL_RenderDrawPoint(renderer,position.x,position.y-1);
        // SDL_RenderDrawPoint(renderer,position.x,position.y+1);
        // SDL_RenderDrawPoint(renderer,position.x+1,position.y+1);
        // SDL_RenderDrawPoint(renderer,position.x+1,position.y-1);
        // SDL_RenderDrawPoint(renderer,position.x-1,position.y+1);
        // SDL_RenderDrawPoint(renderer,position.x-1,position.y-1);
    }

};