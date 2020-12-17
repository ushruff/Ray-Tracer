#ifndef ROTATE_ABOUT_Y_H
#define ROTATE_ABOUT_Y_H
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "TriangleSoup.h"
// Construct a rotation matrix about y (as a 4x4 transformation) given the angle

// Returns 3d Eigen Affine transformation.
void rotation_y(const double theta, Eigen::Matrix3d & R);
void rotate_about_y(const double theta, std::shared_ptr<TriangleSoup>);
#endif
