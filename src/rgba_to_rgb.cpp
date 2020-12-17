#include "rgba_to_rgb.h"
#include <iostream>
using namespace std;

void rgba_to_rgb(
    const std::vector<unsigned char> & rgba,
    const int & width,
    const int & height,
    vector<unsigned char> & rgb)
{
 
    rgb.resize(height*width*3);

    for (int i = 0; i < height; i++)
    {
        int hid = width * i;
        for (int j = 0; j < width; j++){
            
            int id = (hid + j)*3;
            int aid = (hid + j)*4;
            rgb[id] = rgba[aid];
            rgb[id+1] = rgba[aid+1];
            rgb[id+2] = rgba[aid+2];
        }
    }
    
}