#include <iostream>
#include "frameGenerator.h"
#include "fractal.h"

//#define DEFAULT_PARAMS

const std::string NAME = "fractal";

const int WIDTH = 1600;
const int HEIGHT = 900;
const int MAX_ITERATION = 500;
const int BAILOUT = 128;

// Constants needed for fractal generation
#ifdef DEFAULT_PARAMS
/* Default Location - Shows entire Mandelbrot Set*/

const double x = -0.5;
const double y = 0.0;

const double min_x = -2.5;
const double max_x = 1.5;
const double dx = (max_x-min_x)/WIDTH;

#else
/* Custom Location */

const double x = -0.8308158416231485;
const double y =  0.2072176977281955;

const double min_x = -0.83089397771786;
const double max_x = -0.83073770552843;
const double dx = (max_x-min_x)/WIDTH;

#endif

int main(void) {
  SDL_Renderer *renderer;
  SDL_Window *window;

  SDL_Init(SDL_INIT_VIDEO);
  SDL_CreateWindowAndRenderer( WIDTH, HEIGHT, 0, &window, &renderer );

  SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
  SDL_RenderClear(renderer);

  Fractal fractal(x, y, dx, MAX_ITERATION, BAILOUT, renderer, WIDTH, HEIGHT);
  fractal.draw();

  // Just displays fractal name and formula 
  std::cout << fractal << std::endl;

  SDL_RenderPresent(renderer);
  FrameGenerator frameGen(renderer, window, WIDTH, HEIGHT, NAME);
  frameGen.makeFrame();

  SDL_Event event;
  const Uint8* keystate;
  while ( true ) {
    keystate = SDL_GetKeyboardState(0);
    if (keystate[SDL_SCANCODE_ESCAPE]) { break; }
    if (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        break;
      }
    }
  }
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
