//
// Created by shobhit on 18.01.25.
//

#ifndef OPTIMIZATIONBASEDIKWITHOA_H
#define OPTIMIZATIONBASEDIKWITHOA_H

#include <Eigen/Dense>
#include <inverseKinematicsOA_types.h>
#include <vector>
#include <Eigen/Dense>
#include <tuple>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

namespace ObstacleAvoidance {

    class OptimizationBasedIKWithOA {
    public:
        OptimizationBasedIKWithOA(AndreiUtils::ConfigurationParameters const &configOA);


    protected:
        structIkWithOA ok;


    };

} // ObstacleAvoidance

#endif //OPTIMIZATIONBASEDIKWITHOA_H
