#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include "read_json.h"
#include "write_ppm.h"
#include "viewing_ray.h"
#include "raycolor.h"
#include <Eigen/Core>
#include <vector>
#include <iostream>
#include <memory>
#include <limits>
#include <functional>

#include "omp.h"
#include "read_rgba_from_png.h"
#include "rgba_to_rgb.h"

using namespace std;
using namespace Eigen;

int i, j;
vector<unsigned char> rgb_b;
int width =  1810;
int height = 1148;

int main(int argc, char * argv[])
{
  Camera camera;
  std::vector< std::shared_ptr<Object> > objects;
  std::vector< std::shared_ptr<Light> > lights;
  // Read a camera and scene description from given .json file
  read_json(
    argc<=1?"../data/sphere-and-plane.json":argv[1],
    camera,
    objects,
    lights);
	
	
	cout << "Read Json Completed\n";

	// read a RGBA .png
  int width_png,height_png;
  std::vector<unsigned char> rgba;
  read_rgba_from_png("../data/background3.png",rgba,width_png,height_png);

  // Convert to RGB
  rgba_to_rgb(rgba,width_png,height_png,rgb_b);
	
	assert (width_png == width && height_png == height);
	
  std::vector<unsigned char> rgb_image(3*width*height);
  // For each pixel (i,j)

	#pragma omp parallel for
  for(unsigned i=0; i<height; ++i) 
  {
    for(unsigned j=0; j<width; ++j)
    {
      // Set background color
      Eigen::Vector3d rgb(0,0,0);

      // Compute viewing ray
      Ray ray;
      viewing_ray(camera,i,j,width,height,ray);
      
      // Shoot ray and collect color
      bool x = raycolor(ray,1.0,objects,lights,0,rgb);

      // Write double precision color into image
      auto clamp = [](double s){ return std::max(std::min(s,1.0),0.0);};
      rgb_image[0+3*(j+width*i)] = 255.0*clamp(rgb(0));
      rgb_image[1+3*(j+width*i)] = 255.0*clamp(rgb(1));
      rgb_image[2+3*(j+width*i)] = 255.0*clamp(rgb(2));
    }
  }


  write_ppm("../rendered_image/final_image.ppm",rgb_image,width,height,3);
}
