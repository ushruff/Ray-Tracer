#include "viewing_ray.h"

void viewing_ray(
  const Camera & camera,
  const int i,
  const int j,
  const int width,
  const int height,
  Ray & ray)
{
    double p_width, p_height;
    p_width = camera.width;
    p_height = camera.height;
    
    double u = p_width*(j+0.5)/width - p_width/2;
    double v = - p_height*(i+0.5)/height + p_height/2;// changed to match top-left convention
    
    ray.direction = u*camera.u + v*camera.v - camera.w*camera.d;
    ray.origin = camera.e;
}

