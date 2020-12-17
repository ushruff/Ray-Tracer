#include "Triangle.h"
#include "Ray.h"
#include <Eigen/Dense>
#include <Eigen/Core>
#include <Eigen/Geometry>

using namespace std;
using namespace Eigen;

bool Triangle::intersect(
  const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{   
    auto v = corners;
    auto v1 = get<0>(v);
    auto v2 = get<1>(v);
    auto v3 = get<2>(v);
    
    auto re = ray.origin;
    auto rd = ray.direction;
        
    double a = v1[0] - v2[0];
    double d = v1[0] - v3[0];
    double g = rd[0];
    
    double b = v1[1] - v2[1];
    double e = v1[1] - v3[1];
    double h = rd[1];

    double c = v1[2] - v2[2];
    double f = v1[2] - v3[2];
    double i = rd[2];
    
    double j = v1[0] - re[0];
    double k = v1[1] - re[1];
    double l = v1[2] - re[2];

    
    double M = a*(e*i-h*f)+b*(g*f-d*i) + c*(d*h-e*g);

    t = -(f*(a*k-j*b) + e*(j*c-a*l) + d*(b*l-k*c))/M;
    if (t < min_t)
        return false;

    double gamma = (i*(a*k-j*b) + h*(j*c-a*l) + g*(b*l-k*c))/M;
    if (gamma < 0 || gamma > 1)
    	return false;

    double beta = (j*(e*i-h*f) + k*(g*f-d*i) + l*(d*h - e*g))/M;   
    if (beta < 0 || beta > (1-gamma))
        return false;    
    
    //Find normal
    n = (get<1>(v) - get<0>(v)).cross(get<2>(v) - get<0>(v));
    n.normalize();

    return true;
    
}


