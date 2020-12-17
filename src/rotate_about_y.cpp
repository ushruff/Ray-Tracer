#include "rotate_about_y.h"
#include <Eigen/Geometry>

using namespace Eigen;
using namespace std;

void rotation_y(const double theta, Eigen::Matrix3d & R)
{
	R << cos(theta), 0, -sin(theta),
			 0,					 1, 0,
			 sin(theta), 0, cos(theta);
}


//void rotate_about_y(const double theta, std::vector<std::shared_ptr<Object> > & triangles)
//{
//	Matrix3d R;
//	rotation_y(theta, R);
//	int size = triangles.size();
//	std::tuple< Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d> corners;
//	
//	for(int t = 0; t<size; t++)
//	{
//		Vector3d p1, p2, p3;
//		
//		tuple<Vector3d, Vector3d, Vector3d> corners;
//		corners = triangles[t]->corners;
//		
//		p1 = R*get<0>(corners);
//		p2 = R*get<1>(corners);
//		p3 = R*get<2>(corners);
//	}
//	
//}