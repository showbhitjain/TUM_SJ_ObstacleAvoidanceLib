//
// Created by shobhit on 14.11.24.
//

#include <TUM_SJ_ObstacleAvoidanceLib/utilsJointValuesMatlab.h>
#include <privateUtils.h>
#include <add_noise.h>
#include <integrateRungeKutta.h>
#include <rt_nonfinite.h>
#include <utilsMatlab_terminate.h>

using namespace Eigen;
namespace ObstacleAvoidance {

    Eigen::VectorXd integrateConstantRungeKutta(Eigen::VectorXd const &jointVelocity, double const timespan[2],
                                                Eigen::VectorXd const &startJointVector) {

        coder::array<double, 1U> output;
        integrateRungeKutta(EigenVectorToCoder1U(jointVelocity), timespan, EigenVectorToCoder1U(startJointVector),
                            output);
        utilsMatlab_terminate();
        return coder1UtoEigenVector(output);
    }

    Eigen::VectorXd
    addRandomNoisetoJointsSignal(Eigen::VectorXd const &jointsSignal, double const &minNoise, double const &maxNoise) {

        coder::array<double, 1U> output;
        add_noise(EigenVectorToCoder1U(jointsSignal), minNoise, maxNoise, output);
        utilsMatlab_terminate();
        return coder1UtoEigenVector(output);
    }


}// namespace ObstacleAvoidance