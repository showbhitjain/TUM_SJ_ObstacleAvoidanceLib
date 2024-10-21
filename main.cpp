#include <iostream>
#include <Robot.h>
#include <utils.h>
#include <cmath>
using namespace RobotModelling;

int main() {
    std::cout << "Hello, World!" << std::endl;
    AndreiUtils::Posed baseFrame = DualQuaternion<double>::identity();
    auto robot = Robot("../config/robotModellingParameters.json","RobotModelling","PandaRobot",baseFrame);
    Eigen::VectorXd Homejointpositions(robot.getNumberJoints());
    Homejointpositions <<0,0,0,-M_PI/2, 0, M_PI/2, M_PI/4;
    auto trafo = robot.get_transformationmatrix(Homejointpositions,3);

    std::cout<<"The trafo_matrix is: \n"<<trafo<<std::endl;
    auto Jacobi_matrix = robot.jacobian_cartesian(Homejointpositions,3);
    std::cout<<"The Jacobi_matrix is: \n"<<Jacobi_matrix<<std::endl;
    auto Joints = robot.getJoints();

    auto Joints_max_values = Joints->maxValues;

    return 0;

}








