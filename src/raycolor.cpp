#include "raycolor.h"
#include "first_hit.h"
#include "blinn_phong_shading.h"
#include "reflect.h"
#include <iostream>
#include "math.h"
#include "Camera.h"

using namespace std;
using namespace Eigen;

extern vector<unsigned char> rgb_b;
extern Matrix3d inv_TransMatrix;
extern Camera camera_;
extern int width;
extern int height;

int max_depth = 10;

bool refract(const Eigen::Vector3d & d, const Eigen::Vector3d & n, double cl, double ct, Vector3d & tr);

void get_i_j(const Ray & ray, int & i, int & j){
	Vector3d x = (inv_TransMatrix)*ray.direction;
	double u = x(0);
	double v = x(1);
	double p_width = camera_.width;
	double p_height = camera_.height;
	
	double j_ = (u/p_width + 0.5)*width - 0.5;
	double i_ = (-v/p_height + 0.5)*height - 0.5;
	j = (int)round(j_);
	i = (int)round(i_);
}

bool background(const Ray & ray, Vector3d & rgb){
	int i, j;
	get_i_j(ray, i, j);
	bool ret = false;
	if (i <=778)
		ret = true;
	
	if (i < 0)
		i = 0;
	else if (i >= height)
		i = height - 1;
	if (j >= width)
		j = width - (j-width) - 2;
	else if (j < 0)
		j = -j;
	
	int id = (i*width+j)*3;
	rgb = Eigen::Vector3d(rgb_b[id]/255.0,rgb_b[id+1]/255.0,rgb_b[id+2]/255.0);
	return ret;
}

bool raycolor(
  const Ray & ray, 
  const double min_t,
  const std::vector< std::shared_ptr<Object> > & objects,
  const std::vector< std::shared_ptr<Light> > & lights,
  const int num_recursive_calls,
  Eigen::Vector3d & rgb)
{	
	rgb = Eigen::Vector3d(0,0,0);
	
	if (num_recursive_calls > max_depth){
		return false;
	}
  
		
	int hit_id;
	Vector3d n;
	double t;
	
	if (!first_hit(ray, min_t, objects, hit_id, t, n)){
				return false;
	}
	
	shared_ptr<Material> material = objects[hit_id]->material;

	
	//Check if invisible plane is hit
	if (material->ka.norm() == 0.0){
		//Get background colour
		if(background(ray, rgb))
			return true;
	}
	else{
  	rgb += blinn_phong_shading(ray, hit_id, t, n, objects, lights);
	}

	
	double albedo = material->albedo;
    
	double epsilon = 1e-6;
	Ray reflected_ray;
	Vector3d reflected_rgb(0,0,0);
	Vector3d p = ray.origin + ray.direction*t;
	reflected_ray.origin = p;
	reflected_ray.direction = reflect((ray.direction.normalized()), n);
	
	/*Transmission & Refraction*/
	if (albedo != 1.0){/*Object is a dielectric*/
		//indices of refraction
		double cl = objects[hit_id]->material->cl;
		double ct = objects[hit_id]->material->ct;

		Vector3d d = ray.direction;
		double c;
		double kr, kb, kg;
		
		Ray tr_ray;
		tr_ray.origin = p;

		bool x;
		if (d.normalized().dot(n) < 0.0){
			x = refract(d, n, cl, ct, tr_ray.direction); /*Compute refracted Ray*/
			kr = kb = kg = 1.0;
			c = (-d.normalized()).dot(n);
		}	
		else{
			kr = exp(-0.15*t);
			kg = exp(-0.15*t);
			kb = exp(-0.15*t);

			if (refract(d, -n, ct, cl, tr_ray.direction)){
				c = (tr_ray.direction.normalized()).dot(n);
			}
			else{
				Vector3d k(kr,kg,kb);
				raycolor(reflected_ray, epsilon, objects, lights, num_recursive_calls + 1, reflected_rgb);
				rgb += (k.array() * reflected_rgb.array()).matrix();
				return true;
			}
		}

		Vector3d k(kr,kg,kb);
		double R0 = pow((ct-1)/(ct+1),2);
		double R = R0 + (1-R0)*pow(1-c, 5);
		
		Vector3d tr_rgb(0,0,0);
		raycolor(reflected_ray, epsilon, objects, lights, num_recursive_calls + 1, reflected_rgb);
		raycolor(tr_ray, epsilon, objects, lights, num_recursive_calls + 1, tr_rgb);
		rgb += (k.array()*((1-R)*tr_rgb + R*reflected_rgb).array()).matrix();

	}
	else{
		/*Reflection*/
		Vector3d km = objects[hit_id]->material->km;
		if (km.norm() != 0){
			raycolor(reflected_ray, epsilon, objects, lights, num_recursive_calls + 1, reflected_rgb);
			reflected_rgb = (objects[hit_id]->material->km.array() * reflected_rgb.array()).matrix();
		}
		rgb += reflected_rgb;
	}
	
	return true;
}

bool refract(const Eigen::Vector3d & d, const Eigen::Vector3d & n, double cl, double ct, Vector3d & tr){
	
	double r = cl/ct;
	Vector3d l = d.normalized();
	double d_n = l.dot(n);
	
	double D = 1.0-((r*r)*(1.0-d_n*d_n));
	if (D < 0.0){
		return false;
	}
	tr = ((r)*(l-n*d_n) -n*sqrt(D)).normalized();

	return true;
}