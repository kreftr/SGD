#pragma once
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "coordinate.h"
#include <string>
#include <cmath>


class Terrain{

public:
    std::vector<coordinate> terrain_model;

    void generate_model(){

        int interval = 20;
        int height = (rand() % 60 + 60) + 50;
        int steepness;
        int flatten = 6;
        coordinate c;


        for(int i = 0; i < 640; i++){
        
            if (i % interval == 0) steepness = rand() % 6 + (-3);
        
            if(height < 10) steepness = steepness * (-1);

            if(flatten < 0){
                
                height += steepness;

                for(int j = 480 - height; j <= 480; j++){
                    c.x = i - 1;
                    c.y = j;
                    terrain_model.push_back(c);
                }
            
                flatten = 6;
            }
            else {

                for(int j = 480 - height; j <= 480; j++){
                    c.x = i - 1;
                    c.y = j;
                    terrain_model.push_back(c);
                }
                flatten--;
        }
    }

}

    bool is_inside(int pos_x, int pos_y, int rad, int x, int y){
        if(pow((x - pos_x),2) + pow(y - pos_y,2) <= pow(rad,2)) return true;
        else return false;
    }

    void modify_terrain(coordinate pos, int size){
        for(int i = 0; i < terrain_model.size(); i++){
            if(is_inside(pos.x, pos.y, size, terrain_model.at(i).x,terrain_model.at(i).y)){
                terrain_model[i] = terrain_model.back();
                terrain_model.pop_back();
                i--;
            }
        }
    }

    void draw(SDL_Renderer *renderer){
        SDL_SetRenderDrawColor(renderer, 242, 209, 107, 255);
        for(int i = 0; i < terrain_model.size(); i++){
            SDL_RenderDrawPoint(renderer, terrain_model.at(i).x, terrain_model.at(i).y);
        }
}

};