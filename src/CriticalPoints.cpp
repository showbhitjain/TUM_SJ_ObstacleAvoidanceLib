//
// Created by shobhit on 11.12.24.
//

#include <CriticalPoints.h>


CriticalPoints::CriticalPoints(const int &numberOfJoints) :
        distVectorA(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
        distVectorD(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
        distVectorFinalLink(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),


        closestPointObstacleA(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
        closestPointRelativeRobotLinkA(Eigen::MatrixXd::Constant(4, 4, std::numeric_limits<double>::quiet_NaN())),
        closestPointObstacleD(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
        closestPointRelativeRobotLinkD(Eigen::MatrixXd::Constant(4, 4, std::numeric_limits<double>::quiet_NaN())),
        closestPointObstacleFinalLink(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
        closestPointRelativeFinalLink(Eigen::MatrixXd::Constant(4, 4, std::numeric_limits<double>::quiet_NaN())),

        hasCriticalPointA(false),
        hasCriticalPointD(false),
        hasCriticalPointFinalLink(false),

        jacobiDistMaxA(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiDistMaxD(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiDistMaxFinalLink(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),

        jointAnglesCriticalA(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jointAnglesCriticalD(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jointAnglesCriticalFinalLink(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),

        jointVelocityCriticalA(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jointVelocityCriticalD(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jointVelocityCriticalFinalLink(Eigen::VectorXd::Constant(numberOfJoints, std::numeric_limits<double>::quiet_NaN())),

        jacobiDistanceA(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiDistanceD(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiDistanceFinalLink(Eigen::MatrixXd::Constant(1, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),


        jacobiCriticalA(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiCriticalMaxA(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiCriticalD(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiCriticalMaxD(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiCriticalFinalLink(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),
        jacobiCriticalMaxFinalLink(Eigen::MatrixXd::Constant(3, numberOfJoints, std::numeric_limits<double>::quiet_NaN())),

        distance(std::numeric_limits<double>::quiet_NaN()),
        distanceA(std::numeric_limits<double>::quiet_NaN()),
        distanceD(std::numeric_limits<double>::quiet_NaN()),
        distanceFinalLink(std::numeric_limits<double>::quiet_NaN())

        {}
