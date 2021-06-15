#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

#include "terrain.h"
#include "tank.h"
#include "bullet.h"
#include "power_bar.h"


#define errcheck(e)                                                            \
  {                                                                            \
    if (e) {                                                                   \
      cout << SDL_GetError() << endl;                                          \
      SDL_Quit();                                                              \
      return -1;                                                               \
    }                                                                          \
  }


int main(int , char **) {

  using namespace std;
  using namespace std::chrono;
  srand (time(NULL));
  const int width = 640;
  const int height = 480;
  

  errcheck(SDL_Init(SDL_INIT_VIDEO) != 0);

  SDL_Window *window = SDL_CreateWindow(
      "Game", SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
  errcheck(window == nullptr);

  SDL_Renderer *renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED); 
  errcheck(renderer == nullptr);


  milliseconds dt(15);

  SDL_Surface* surface = IMG_Load("explosion.png"); 
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); 
  SDL_FreeSurface(surface);
  SDL_Rect img_destination;
  img_destination.h = 50;
  img_destination.w = 50;

  Terrain terrain;
  Tank tank(100, 250);
  PowerBar power_bar(width, 60, 30);
  std::vector<Bullet> bullets;
  terrain.generate_model();
  tank.spawn();
  
  bool is_not_falling = true;
  bool draw_power_bar = false;
  bool explosion = false;

  const int bullet_power [4] = {5, 10, 15, 20};
  int bullet_index = 0;
  float gun_power = 1;
  int counter = 0;

  steady_clock::time_point current_time = steady_clock::now(); // remember current time
  for (bool game_active = true; game_active;) {
    SDL_Event event;

    SDL_RenderClear(renderer);

    

    while (SDL_PollEvent(&event)) {
      
      if (event.type == SDL_QUIT) game_active = false;

      if(is_not_falling){
        if(event.type == SDL_KEYDOWN){
          switch (event.key.keysym.scancode){
            case SDL_SCANCODE_RIGHT:
              tank.move_right(terrain.terrain_model);
              break;
            case SDL_SCANCODE_LEFT:
              tank.move_left(terrain.terrain_model);
              break;
            case SDL_SCANCODE_UP:
              tank.barrel_up();
              break;
            case SDL_SCANCODE_DOWN:
              tank.barrel_down();
              break;
            case SDL_SCANCODE_F:
              if(bullet_index + 1 <= 3){
                bullet_index++;
              }else bullet_index = 0;
              system("clear");
              cout<<"Bullet power: "<<bullet_power[bullet_index]<<endl;
              break;
            case SDL_SCANCODE_SPACE:
              if(gun_power < 5){
                gun_power = gun_power + 0.05;
                power_bar.move_pointer();
              }
              draw_power_bar = true;
              break;
            default:
              break;
          }
        }else if (event.type == SDL_KEYUP){
          switch (event.key.keysym.scancode){
            case SDL_SCANCODE_SPACE:
              bullets.push_back(Bullet(tank.barrel_position, tank.angle, bullet_power[bullet_index], gun_power));
              cout<<gun_power<<endl;
              power_bar.reset_pointer();
              draw_power_bar = false;
              gun_power = 0;
              break;
            default:
              break;
          }
        }
      }
      

    }
    
    is_not_falling = tank.gravity(terrain.terrain_model);



    for(int i = 0; i < bullets.size(); i++){

      if(bullets.at(i).touched_groud(terrain.terrain_model)){
        img_destination.x = bullets.at(i).position.x - img_destination.w/2;
        img_destination.y = bullets.at(i).position.y - img_destination.h/2;
        switch (bullets.at(i).power)
        {
        case 3:
          img_destination.h = 20;
          img_destination.w = 20;
          break;
        case 5:
          img_destination.h = 25;
          img_destination.w = 25;
          break;
        case 10:
          img_destination.h = 30;
          img_destination.w = 30;
          break;
        case 15:
          img_destination.h = 35;
          img_destination.w = 35;
          break;
        case 20:
          img_destination.h = 40;
          img_destination.w = 40;
          break;
        default:
          break;
        }
        explosion = true;
        terrain.modify_terrain(bullets.at(i).position, bullets.at(i).power);
        if(terrain.is_inside(bullets.at(i).position.x, bullets.at(i).position.y, bullets.at(i).power, tank.position.x, tank.position.y)){
          tank.health = tank.health - bullets.at(i).power;
          cout<<"Health: "<<tank.health<<endl;
          if(tank.health <= 0) game_active = false;
        }
        bullets[i] = bullets.back();
        bullets.pop_back();
        i--;
      }
      else if(bullets.at(i).is_out_of_boundaries()){
        bullets[i] = bullets.back();
        bullets.pop_back();
        i--;
      }
      else bullets.at(i).draw(renderer);

    }


    if(draw_power_bar) power_bar.draw(renderer);
    terrain.draw(renderer);
    tank.draw(renderer);
    
    SDL_SetRenderDrawColor(renderer, 138, 183, 248, 255);
    if(explosion){
      SDL_RenderCopy(renderer, texture, NULL, &img_destination);
      img_destination.h = img_destination.h + 3;
      img_destination.w = img_destination.w + 3;
      counter++;
      if(counter > 5) {
        img_destination.h = img_destination.h - 15;
        img_destination.w = img_destination.w - 15;
        explosion = false;
        counter = 0;
      }
    }
    SDL_RenderPresent(renderer);
    this_thread::sleep_until(current_time = current_time + dt);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}