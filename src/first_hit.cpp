#include "first_hit.h"
#include <iostream>
#include <limits>

using namespace std;

bool first_hit(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  int & hit_id, 
  double & t,
  Eigen::Vector3d & n)
{
    
    double inf = numeric_limits<double>::infinity();
    hit_id = -1;
    t = inf;
    double local_t;
    Eigen::Vector3d local_n;
    
    int num_objects = objects.size();
    
    for(int i = 0; i < num_objects; i++){
        if (objects[i]->intersect(ray, min_t, local_t, local_n)){
            if (local_t < t){
                t = local_t;
                n = local_n;
                hit_id = i;
            }
        }
    } 
    if (hit_id != -1) return true;
    else return false;
    
}

