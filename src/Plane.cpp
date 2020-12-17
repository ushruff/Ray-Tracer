#include "Plane.h"
#include "Ray.h"

bool Plane::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    
    double denom = normal.dot(ray.direction);
    if (denom == 0)
        return false;
    
    double q = normal.dot(point);
    double num = q - normal.dot(ray.origin);
    
    
    t = num/denom;
    n = normal;
    n.normalize();
    if (t < min_t)
        return false;
    else return true;
    
  
}

