#include <SDL2/SDL.h>
#include <vector>
#include <cmath>

#define PI 3.14159265

float distance(coordinate a, coordinate b)
{
    return sqrt(pow(a.x - b.x, 2) +
                pow(a.y - b.y, 2) * 1.0);
}


class Tank {

public:
    coordinate position;
    std::vector<coordinate> tank_model;

    coordinate barrel_position;
    double angle = 0.0;
    int barrel_length = 8;

    Tank(int x, int y){
        position.x = x;
        position.y = y;
    }

    
    void spawn(){
        coordinate c;
        for(int h = 0; h < 4; h++){                                                 
            for(int w_r = 0; w_r < 3; w_r++){
                c.x = position.x + w_r;
                c.y = position.y - h; 
                tank_model.push_back(c);      
            }
            for(int w_l = 1; w_l < 4; w_l++){
                c.x = position.x - w_l;
                c.y = position.y - h; 
                tank_model.push_back(c);      
            }   
        }    

        barrel_position.x = position.x;
        barrel_position.y = position.y - 3;    
    }

    void move_right(std::vector<coordinate> terrain_model){
        coordinate c;
        int elevation = 0;

        for(int i = 0; i < terrain_model.size(); i++){
            if((position.x + 1 == terrain_model.at(i).x) && position.y >= terrain_model.at(i).y) elevation++;
        }

        move_up(elevation);

        for(int i = 0; i < tank_model.size(); i++){
            c.x = tank_model.at(i).x + 1;
            c.y = tank_model.at(i).y;
            tank_model.at(i) = c;
        }
        position.x++;
        barrel_position.x++;
    }

    void move_left(std::vector<coordinate> terrain_model){
        coordinate c;
        int elevation = 0;

        for(int i = 0; i < terrain_model.size(); i++){
            if((position.x - 1 == terrain_model.at(i).x) && position.y >= terrain_model.at(i).y) elevation++;
        }

        move_up(elevation);

        for(int i = 0; i < tank_model.size(); i++){
            c.x = tank_model.at(i).x - 1;
            c.y = tank_model.at(i).y;
            tank_model.at(i) = c;
        }
        position.x--;
        barrel_position.x--;
    }

    void move_down(){
        coordinate c;
        for(int i = 0; i < tank_model.size(); i++){
            c.x = tank_model.at(i).x;
            c.y = tank_model.at(i).y + 1;
            tank_model.at(i) = c;
        }
        position.y++;
        barrel_position.y++;
    }

    void move_up(int elevation_level){
        coordinate c;
        for(int i = 0; i < tank_model.size(); i++){
            c.x = tank_model.at(i).x;
            c.y = tank_model.at(i).y - elevation_level;
            tank_model.at(i) = c;
        }
        position.y = position.y - elevation_level;
        barrel_position.y = barrel_position.y - elevation_level;
    }

    void barrel_up(){
        angle = angle - 10;
    }

    void barrel_down(){
        angle = angle + 10;
    }

    bool gravity(std::vector<coordinate> terrain){
        float min_distance = 999.99;
        for(int i = 0; i < terrain.size(); i++){
            if(distance(position, terrain.at(i)) < min_distance){
                min_distance = distance(position, terrain.at(i));
            }            
        }
        
        if(min_distance > 1){
            move_down();
            return false;
        }
        else return true;
    }

    void draw(SDL_Renderer *renderer){
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        for(int i = 0; i < tank_model.size(); i++){
            SDL_RenderDrawPoint(renderer, tank_model.at(i).x, tank_model.at(i).y);
        }
        
        SDL_SetRenderDrawColor(renderer, 105, 105, 105, 255);
        SDL_RenderDrawLine(renderer, barrel_position.x, barrel_position.y,
         cos(angle*PI/180)*barrel_length + barrel_position.x, sin(angle*PI/180)*barrel_length + barrel_position.y);
    }

};