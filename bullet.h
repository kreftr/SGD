#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

#define PI 3.14159265

class Bullet{

public:

    coordinate position;
    float velocity;
    float vx, vy;
    int angle;
    float time = 0;
    float g = 9.8;
    int power;

    Bullet(coordinate pos, double a, int p){
        position.x = pos.x + cos(a*PI/180)*5;
        position.y = pos.y + sin(a*PI/180)*5;
        angle = a;
        velocity = 40;
        vx = velocity * cos(angle*PI/180);
        vy = velocity * sin(angle*PI/180);
        power = p;
    }


    void draw(SDL_Renderer *renderer){
        
        position.x += cos(angle*PI/180) * 5;
        position.y += (0.5 * g * time * time + velocity * sin(angle*PI/180) * time*0.05); 
        
        time = time + 0.015;
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(renderer,position.x,position.y);
        
        SDL_RenderDrawPoint(renderer,position.x+1,position.y);
        SDL_RenderDrawPoint(renderer,position.x-1,position.y);
        SDL_RenderDrawPoint(renderer,position.x,position.y-1);
        SDL_RenderDrawPoint(renderer,position.x,position.y+1);
        SDL_RenderDrawPoint(renderer,position.x+1,position.y+1);
        SDL_RenderDrawPoint(renderer,position.x+1,position.y-1);
        SDL_RenderDrawPoint(renderer,position.x-1,position.y+1);
        SDL_RenderDrawPoint(renderer,position.x-1,position.y-1);
    }

    bool is_out_of_boundaries(){
        if(position.x > 640 || position.x < 0) return true;
        else if(position.y > 480 || position.y < 0) return true;
        else return false;
    }

    bool touched_groud(std::vector<coordinate> terrain){
        for(int i = 0; i < terrain.size(); i++){
            if(position.x == terrain.at(i).x && position.y == terrain.at(i).y) return true;
        }
        return false;
    }
};