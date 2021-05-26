#include <SDL2/SDL.h>
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


  Terrain terrain;
  terrain.generate_model();
  Tank tank(100, 250);
  tank.spawn();
  std::vector<Bullet> bullets;
 
  bool is_not_falling = true;


  steady_clock::time_point current_time = steady_clock::now(); // remember current time
  for (bool game_active = true; game_active;) {
    SDL_Event event;

    SDL_RenderClear(renderer);

    

    while (SDL_PollEvent(&event)) {
      
      if (event.type == SDL_QUIT) game_active = false;

      if(event.type == SDL_KEYDOWN){
        if(is_not_falling){
          switch (event.key.keysym.scancode)
          {
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
          case SDL_SCANCODE_SPACE:
            bullets.push_back(Bullet(tank.barrel_position, tank.angle));
            break;
          default:
            break;
          }
        }
      }
      

    }
    
    is_not_falling = tank.gravity(terrain.terrain_model);
    
    if(bullets.size() > 0){
      int size = bullets.size();
      for(int i=0; i < size; i++){
        // if((bullets.at(i).position.x > 640 || bullets.at(i).position.x < 0) || (bullets.at(i).position.y > 480 || bullets.at(i).position.y < 0)){
        //   bullets.erase(bullets.begin()+i);
        //   size--;
        //   if(size == 0) break;
        // }
        bullets.at(i).draw(renderer);
      }
    }

    terrain.draw(renderer);
    tank.draw(renderer);
    
    SDL_SetRenderDrawColor(renderer, 138, 183, 248, 255);
    SDL_RenderPresent(renderer);
    this_thread::sleep_until(current_time = current_time + dt);
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}