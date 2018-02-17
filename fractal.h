#include <string>
#include <SDL2/SDL.h>

class Fractal {
public:
  Fractal(double, double, double, int, int, SDL_Renderer*, int, int);
  Fractal(Fractal&) = delete;
  void draw() const;
  Fractal& operator=(const Fractal&) = delete;
  friend std::ostream& operator<<(std::ostream&, const Fractal&);

private:
  const double center_x, center_y;
  const double dx;
  const int max_iteration;
  const int bailout;
  SDL_Renderer *renderer;
  const int width, height;
  const double min_x, max_y;
  const std::string name, formula;
};
