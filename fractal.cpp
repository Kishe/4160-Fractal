#include <cmath>
#include <complex>
#include "fractal.h"
#include "cubichermitespline.h"

Fractal::Fractal(double x, double y, double dx_, int max_iter, int bail,
        SDL_Renderer* rend, int w, int h) :
        center_x(x), center_y(y), dx(dx_),
        max_iteration(max_iter), bailout(bail),
        renderer(rend), width(w), height(h),
        min_x(center_x-dx_*width/2), max_y(center_y+dx_*height/2),
		name("Mandelbrot Set"), formula("f(z) = z^2 + c")
{ }

std::ostream& operator<<(std::ostream& o, const Fractal& f) {
  return o << "Fractal name: " << f.name << std::endl << "Formula: "
	   << f.formula << std::endl;
}

void Fractal::draw() const {
  /* First, create the splines needed to have a smooth color gradient for the
     fractal.

     The color arrays (redVals, greenVals, blueVals) hold the corresponding
     red, green, and blue values for 5 different colors that will help create
     the gradient. There is a sixth entry that is equal to the first so there
     is a smooth transition from the last color back to the first.

     The array x[] holds the values for each color's horizontal position on
     gradient. Ranges from 0 to 399, with 399 representing the first color as
     above.
  */

  // Last index needs to be the same as the first
  double redVals[] =   {  0,  32, 237, 255,   0,   0};
  double greenVals[] = {  7, 107, 255, 170,   2,   7};
  double blueVals[] =  {100, 203, 255,   0,   0, 100};
  double x[] =         {  0,  64, 168, 257, 343, 399};

  // Create the rgb splines. They allow us to determine what color should be
  // at any given value of x
  MonotoneCubicInterpolation redSpline(&x[0], &redVals[0], 6);
  MonotoneCubicInterpolation greenSpline(&x[0], &greenVals[0], 6);
  MonotoneCubicInterpolation blueSpline(&x[0], &blueVals[0], 6);

  // Pass over each pixel in the window and draw it accordingly
  for(int i=0; i<width; i++) {
    for(int j=0; j<height; j++) {
      int iteration = 0;
      double x_init = min_x + dx*i;
      double y_init = max_y - dx*j;

      double x = 0.0;
      double y = 0.0;

      // Iterate the fractal's function (z = z^2 + c) until a predetermined
      // value is reached (bailout) or until the maximum number of iterations
      // is reached
      while((x*x + y*y) < bailout && iteration < max_iteration) {
        double tempx = x*x - y*y + x_init;
        y = 2*x*y + y_init;
        x = tempx;
        iteration++;
      }

      // Only color the pixels that DO NOT correspond to points
      // that belong to the Mandelbrot Set with the gradient.
      if(iteration < max_iteration) {
      // This is the default 'smooth' coloring formula for the Mandelbrot Set
      // in Ultra Fractal 4 (fractal drawing software)

        // Final x and y values as a complex number
        std::complex<double> z(x, y);

        // The degree of the fractal's formula as a complex number
        std::complex<double> p(2, 0);

        // The inverse log of the above
        std::complex<double> il(pow(log(p), -1));

        // Double log of the bailout value
        double lp(log(log(bailout)));

        // Actual formula for determining a point's index in the gradient,
        // which is then used to determine its actual color
        double color_ndx = 0.05*std::real((double)iteration+il*lp-il*log((log(abs(z)))));

        /* Transfer Function: modifies color_ndx before getting color
           values. Comment out for "Linear transfer" (calculated index is the
           value actually used in the gradient). Logarithmic transfer is
           probably the best looking for this fractal and coloring formula.
           Other good functions include sqrt(), cbrt(), and arctan(). */
        color_ndx = log(color_ndx);

        // Get the appropriate rgb values
        double red, green, blue;
        red = redSpline((int)(color_ndx*400)%400);
        green = greenSpline((int)(color_ndx*400)%400);
        blue = blueSpline((int)(color_ndx*400)%400);

        // Boundary fixing
        if(red > 255) red = 255; if(red < 0) red = 0;
        if(green > 255) green = 255; if(green < 0) green = 0;
        if(blue > 255) blue = 255; if(blue < 0) blue = 0;

        SDL_SetRenderDrawColor(renderer, (int)red, (int)green, (int)blue, 255);
      }
      // Pixel corresponds to a point that belongs to the set, so
      // color it black.
      else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      }

      // Draw the pixel
      SDL_RenderDrawPoint(renderer, i, j);
    }
  }

  // Uncomment if you just want the fractal
  // (i.e., no complex plane axes or gradient in bottom left corner)
  //return;


  // Draw complex plane axes
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect hzntl = {30, 10, width-40, 2};
  SDL_RenderFillRect(renderer, &hzntl);
  SDL_Rect vrtcl = {width-10, 10, 2, height-40};
  SDL_RenderFillRect(renderer, &vrtcl);

  for(int i=30; i<width-10; i+=(width-40)/5) {
    SDL_Rect rect = {i, 10, 2, 10};
    SDL_RenderFillRect(renderer, &rect);
  }
  for(int j=(height-40)/5+10; j<height; j+=(height-40)/5) {
    SDL_Rect rect = {width-20, j, 10, 2};
    SDL_RenderFillRect(renderer, &rect);
  }


  // Draw the color gradient in bottom left corner
  //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Rect rect = {0, height-104, 404, 104};
  SDL_RenderFillRect(renderer, &rect);

  for(int i=2; i<402; i++) {
    double red, green, blue;
    red = redSpline(i-2);
    green = greenSpline(i-2);
    blue = blueSpline(i-2);
    if(red > 255) red = 255; if(red < 0) red = 0;
    if(green > 255) green = 255; if(green < 0) green = 0;
    if(blue > 255) blue = 255; if(blue < 0) blue = 0;

    SDL_SetRenderDrawColor(renderer, red, green, blue, 255);
    SDL_RenderDrawLine(renderer, i, height-102, i, height-3);
  }
}
