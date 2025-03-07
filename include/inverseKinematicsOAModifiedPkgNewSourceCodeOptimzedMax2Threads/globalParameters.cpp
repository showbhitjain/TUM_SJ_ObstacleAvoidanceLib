//
// Created by shobhit on 06.03.25.
//

#include "globalParameters.h"

void OAGlobals::setConstraintTolerance(double newTolerance) {
    OAGlobals::constraintTolerance = newTolerance;
}

void OAGlobals::setOptimalityTolerance(double newOptimalityTolerance) {
    OAGlobals::optimalityTolerance = newOptimalityTolerance;
}

void OAGlobals::setStepTolerance(double newStepTolerance) {
    OAGlobals::stepTolerance = newStepTolerance;

}

void OAGlobals::setMaxIterations(int newMaxIterations) {
    OAGlobals::maxIterations = newMaxIterations;
}

int OAGlobals::maxIterations = 400;
double OAGlobals::optimalityTolerance = 1.0E-6;
double OAGlobals::stepTolerance = 1.0E-6;
double OAGlobals::constraintTolerance = 1.0E-6;

