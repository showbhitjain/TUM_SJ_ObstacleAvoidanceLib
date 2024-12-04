//
// Created by shobhit on 30.11.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H


#include <Eigen/Dense>
#include <string>


class Obstacles {
public:
    Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
               Eigen::Vector4d orientation = {1,0,0,0},  Eigen::Vector3d axis = {0,0,0});

    std::tuple<Eigen::VectorXd,Eigen::VectorXd,double> distanceCalculation(Obstacles const &otherObstacle);



protected:
    std::string obstacleType;
    Eigen::VectorXd Center;
    Eigen::VectorXd dimensions;
    Eigen::Vector4d orientation;
    Eigen::Vector3d axis;
    std::string obstacleName;

};


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_OBSTACLES_H
