#include <vector>

class PowerBar{
public:

    std::vector<coordinate> power_bar_model;
    float pointer;

    PowerBar(int width, int height, int offset){
        coordinate c;
        pointer = width/2 - 127;

        for(offset; offset < height; offset++){
            c.r = 0;
            c.g = 255;
            c.b = 0;
            pointer = width/2 - 127;
            for(int j = width/2 - 127; j < width/2 + 127; j++){
                c.y = offset;
                c.x = j;
                if(j % 2 == 0){
                    if(c.x < width/2){
                        c.r = c.r + 3;
                    }
                    else c.g = c.g - 3;
                    power_bar_model.push_back(c);
                } else power_bar_model.push_back(c);
            }
        }
    }

    void move_pointer(){
        if(pointer < 320 + 127) pointer = pointer + 2.54;
    }

    void reset_pointer(){
        pointer = 320 - 127;
    }

    void draw(SDL_Renderer *renderer){
        for(int i =0; i < power_bar_model.size(); i++){
            SDL_SetRenderDrawColor(renderer, power_bar_model.at(i).r, power_bar_model.at(i).g, power_bar_model.at(i).b, 255);
            SDL_RenderDrawPoint(renderer,power_bar_model.at(i).x,power_bar_model.at(i).y);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

        for (int h = 30; h < 60; h++)
        {
            SDL_RenderDrawPoint(renderer,pointer+1,h);
            SDL_RenderDrawPoint(renderer,pointer,h);
            SDL_RenderDrawPoint(renderer,pointer-1,h);
        }
        
    }
};