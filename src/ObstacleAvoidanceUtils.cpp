//
// Created by shobhit on 26.11.24.
//

#include "ObstacleAvoidanceUtils.h"
#include <cmath>


namespace ObstacleAvoidance {

    double smoothingConstraintScheme(double d, double dStop, double dActivate, double &k) {
// Calculate d0 based on dStop, dActivate, and k
        double d0 = (dStop + dActivate) / k;

// Check if k is within the valid range, and correct it if not
        double kMin = (dStop + dActivate) / dActivate;
        double kMax = (dStop + dActivate) / dStop;

// If k is not valid, set it to the closest bound
        if (k < kMin) {
            k = kMin;
            d0 = (dStop + dActivate) / k; // Recalculate d0 with the new k value
        } else if (k > kMax) {
            k = kMax;
            d0 = (dStop + dActivate) / k; // Recalculate d0 with the new k value
        }

// Determine the value of s based on the condition of d
        if (d >= dActivate) {
            return 1;
        } else if (d > d0 && d < dActivate) {
            return cos(M_PI * (d - d0) / (dActivate - d0) + M_PI);
        } else if (d <= d0) {
            return -1;
        } else {
// This part of the code should never be reached with valid inputs
            return NAN; // Return NaN if the input does not match any condition
        }
    }

    //convert obstacles vector into map by assigning the keys as index of obstacles Array
    std::map<std::string, Obstacles> conversionObstaclesVectorToMap(std::vector<Obstacles> const &obstaclesArray) {
        std::map<std::string, Obstacles> obstaclesMap;
        for (size_t i = 0; i < obstaclesArray.size(); ++i) {
            obstaclesMap["Obstacle" + std::to_string(i)] = obstaclesArray[i];
        }
        return obstaclesMap;

    }
}


