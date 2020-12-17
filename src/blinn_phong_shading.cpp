#include "blinn_phong_shading.h"
#include "math.h"
// Hint:
#include "first_hit.h"
#include <iostream>

using namespace std;
using namespace Eigen;

Eigen::Vector3d blinn_phong_shading(
    const Ray & ray,
    const int & hit_id, 
    const double & t,
    const Eigen::Vector3d & n,
    const std::vector< std::shared_ptr<Object> > & objects,
    const std::vector<std::shared_ptr<Light> > & lights)
{
    double epsilon = 1e-6;
    Vector3d L = Vector3d (0,0,0);
    shared_ptr<Material> material = objects[hit_id]->material;
    Vector3d ka, kd, ks;
    ka = material->ka;
    kd = material->kd;
    ks = material->ks;
    double phong_exp = material->phong_exponent;
    
    L+= 0.1*ka; /*Ambient light*/
    
    double max_t, shadow_t;
    Vector3d v, h, placeholder1;
    Ray l;
    int placeholder;
    int num_lights = lights.size();
    v = (-ray.direction).normalized();
    l.origin = ray.origin + t*ray.direction;
    
		#pragma omp parallel for
    for (int i = 0; i < num_lights; i++){
			/*Check for shadows*/     
			lights[i]->direction(l.origin, l.direction, max_t);
			int s_hit_id;
			bool hit = first_hit(l, epsilon, objects, s_hit_id, shadow_t, placeholder1);

			if (!hit || shadow_t > max_t){/*Enter if No shadow*/   
					Vector3d I = lights[i]->I; 
					L+= (kd.array()*I.array()).matrix()*max(0.0, n.dot(l.direction)); /*Diffuse lighting*/
					h = (v + l.direction).normalized();/*Compute half-vector*/
					L+= (ks.array()*I.array()).matrix()*pow(max(0.0, n.dot(h)), phong_exp); /*Specular lighting*/
			}
			else if (hit){
					double albedo = objects[s_hit_id]->material->albedo;
					if (albedo == 1.0)
						continue;
					else{
											//Vector3d I = lights[i]->I; 
					//L+= (1-albedo)*(kd.array()*I.array()).matrix()*max(0.0, n.dot(l.direction)); /*Diffuse lighting*/
					//h = (v + l.direction).normalized();/*Compute half-vector*/
					//L+= (1-albedo)*(ks.array()*I.array()).matrix()*pow(max(0.0, n.dot(h)), phong_exp); /*Specular lighting*/	
					}
			}
    }
    
    return (L);
}