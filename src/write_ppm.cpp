#include "write_ppm.h"
#include <fstream>
#include <cassert>
#include <iostream>

using namespace std;

bool write_ppm(
  const std::string & filename,
  const std::vector<unsigned char> & data,
  const int width,
  const int height,
  const int num_channels)
{
  assert((num_channels == 3 || num_channels == 1 ));

    ofstream ppm_file;
    ppm_file.open (filename);
    
    if (!ppm_file.is_open()) return false; /*File failed to open */
    
    if (num_channels == 3)
        ppm_file << "P3" << endl << width << " " << height << endl << "255" << endl;
    else
        ppm_file << "P2" << endl << width << " " << height << endl << "255" << endl;    

    int i;
    for (i = 0; i < height; i++)
    {
        int hid = width * i;
        for (int j = 0; j < width; j++){
            int id = (hid + j)*num_channels;
            if (num_channels == 3)
                ppm_file << (double)data[id] << " " << (double)data[id+1] << " " << (double)data[id+2] << " ";
            else
                ppm_file << (double)data[id] << " ";   
        
        }
        ppm_file << "\n";
    }
   
  return true;
}
