//
// Created by shobhit on 30.11.24.
//

#include <Obstacles.h>

Obstacles::Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
                      Eigen::Vector4d orientation,  Eigen::Vector3d axis) : obstacleType(obstacleType),
                                                                                        Center(center),
                                                                                        dimensions(dimensions),
                                                                                        orientation(orientation),
                                                                                        axis(axis) {



}
