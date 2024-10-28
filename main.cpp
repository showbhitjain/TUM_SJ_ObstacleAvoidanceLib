#include <iostream>
#include <Robot.h>
#include <utils.h>
#include <cmath>
#include <coder_array.h>
#include <Trajectory.h>

using namespace RobotModelling;
using namespace trajectoryGeneration;
using namespace  std;

int main() {
    /*vector<double> g ;

    coder::array<double,2> xd;

    std::shared_ptr<Trajectory> t;

    t =  make_shared<Trajectory>();
    t->cartesian_trajectory()*/

    std::shared_ptr<Eigen::Vector3d> x = std::make_shared<Eigen::Vector3d>();
    (*x) << 0, 1, 2;
    cout << x << endl;
    cout << x->transpose() << endl;
    auto y = x;  // copy
    cout << x << endl;
    auto z = std::move(x); // move
    auto w = x;  // copy after move
    cout << x << " " << z << endl;
    cout << y->transpose() << "; " <<  z->transpose() << endl;
    cout << x << " " << w << endl;

    std::cout << "Hello, World!" << std::endl;
    AndreiUtils::Posed baseFrame = DualQuaternion<double>::identity();
    auto robot = Robot("../config/robotModellingParameters.json","RobotModelling","PandaRobot",baseFrame);
    Eigen::VectorXd Homejointpositions(robot.getNumberJoints());
    Homejointpositions <<0,0,0,-M_PI/2, 0, M_PI/2, M_PI/4;
    auto trafo = robot.forwardKinematics(Homejointpositions, 7);

    std::cout<<"The trafo_matrix is: \n"<<trafo<<std::endl;
    auto Jacobi_matrix = robot.jacobianCartesian(Homejointpositions, 7);
    std::cout<<"The Jacobi_matrix is: \n"<<Jacobi_matrix<<std::endl;
    Eigen::Vector3d trvec(0,0,0.103399);
    Eigen::Vector3d eul(0,0,-M_PI_4);
    auto tform = trvec2tform(trvec)*convertEulerToTransform(eul);
    std::cout<<"The trafo_matrix between endeffector and Tcp is: \n"<<tform<<std::endl;
    auto Joints = robot.getJoints();

    auto Joints_max_values = Joints->maxValues;

    return 0;

}








