//
// Created by shobhit on 06.03.25.
//

#ifndef GLOBALPARAMETERS_H
#define GLOBALPARAMETERS_H

namespace OAGlobals {
    void setConstraintTolerance(double newTolerance);

    void setOptimalityTolerance(double newOptimalityTolerance);

    void setStepTolerance(double newStepTolerance);

    void setMaxIterations(int newMaxIterations);

    extern double constraintTolerance;
    extern int maxIterations;
    extern double stepTolerance;
    extern double optimalityTolerance;
}

#endif //GLOBALPARAMETERS_H
