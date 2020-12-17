#include "Sphere.h"
#include "Ray.h"
#include "math.h"
#include <iostream>

using namespace std;

bool Sphere::intersect(
    const Ray & ray, const double min_t, double & t, Eigen::Vector3d & n) const
{
    double a, b, c;
    double t1;
    
    Eigen::Vector3d e = ray.origin; 
    Eigen::Vector3d rd = ray.direction; 
    
    a = (rd).dot(rd);
    b = 2*(e-center).dot(rd);
    c = (e-center).dot(e-center) - radius*radius;
    
    double D = b*b - 4*a*c;//discriminant
    
    if (D < 0)//no solution
        return false;
    else if (D == 0){ //1 solution
        t1 = -b/(2*a);
        if (t1 < min_t) return false;
        t = t1;
        n = ((e + t*rd) - center)/radius;
    }
    else if(D > 0){ //2 solutions
        double t1 = (-b - sqrt(D))/(2*a); //Closer solution
        
        if (t1 < min_t)
            t1 = (-b + sqrt(D))/(2*a);
        
        if (t1 < min_t) return false;
        
        t = t1;
        n = ((e + t*rd) - center)/radius;   
				//n.normalize();
    }
    
    return true;
}

