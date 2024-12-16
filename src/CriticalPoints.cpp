//
// Created by shobhit on 11.12.24.
//

#include <CriticalPoints.h>


CriticalPoints::CriticalPoints(const int &numberLinks) :
    distVectorA(Eigen::MatrixXd::Constant(3,1,std::numeric_limits<double>::quiet_NaN()) ),
    distVectorD(Eigen::MatrixXd::Constant(3, 1, std::numeric_limits<double>::quiet_NaN())),
    hasCriticalpointsA(false),
    hasCriticalpointsD(false),
    jacobiDistMaxA(Eigen::MatrixXd::Constant(3, numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jacobiDistMaxD(Eigen::MatrixXd::Constant(3, numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jointAnglesCriticalA(Eigen::VectorXd::Constant(numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jointAnglesCriticalD(Eigen::VectorXd::Constant(numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jointVelocityCriticalA(Eigen::VectorXd::Constant(numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jointVelocityCriticalD(Eigen::VectorXd::Constant(numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jacobiDistanceA(Eigen::MatrixXd::Constant(3, numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jacobiDistanceD(Eigen::MatrixXd::Constant(3, numberLinks, std::numeric_limits<double>::quiet_NaN())),
    jacobiCriticalA(Eigen::MatrixXd::Constant(3, numberLinks,  std::numeric_limits<double>::quiet_NaN())),
    jacobiCriticalMaxA(Eigen::MatrixXd::Constant(3, numberLinks,  std::numeric_limits<double>::quiet_NaN())),
    jacobiCriticalD(Eigen::MatrixXd::Constant(3, numberLinks,  std::numeric_limits<double>::quiet_NaN())),
    jacobiCriticalMaxD(Eigen::MatrixXd::Constant(3, numberLinks,  std::numeric_limits<double>::quiet_NaN())),
    distance(0.0)
{}
