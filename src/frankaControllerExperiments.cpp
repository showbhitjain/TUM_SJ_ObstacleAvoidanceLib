//
// Created by shobhit on 06.12.24.
//


#include <utils.h>
#include <Eigen/Dense>
#include <iostream>
#include <controllerFranka.h>


using namespace ObstacleAvoidance;
using namespace Eigen;

int main() {
    auto desiredJointValuesMatrix = readMatrixFromCSV("../output.csv");

    try {
        franka::Robot realRobot("192.168.5.10");
        setDefaultBehavior(realRobot);


        Vector7d firstEigenJointPosition = desiredJointValuesMatrix(all, 0);
        std::array<double, 7> firstJointPosition{};
        std::copy_n(firstEigenJointPosition.data(), 7, firstJointPosition.begin());

        MotionGenerator motion_generator(0.3, firstJointPosition);
        std::cout << "WARNING: This example will move the robot! "
                << "Please make sure to have the user stop button at hand!" << std::endl
                << "Press Enter to continue..." << std::endl;
        std::cin.ignore();
        realRobot.control(motion_generator);
        std::cout << "Finished moving to initial joint configuration." << std::endl;

        // Load the kinematics and dynamics model.
        franka::Model model = realRobot.loadModel();

        size_t index = 0;

        //set internal jointImpedance for internal controller
        realRobot.setJointImpedance({{1, 1, 1, 1, 1, 1, 1}});
        //realRobot.setCartesianImpedance({{3000, 3000, 3000, 300, 300, 300}});
        realRobot.setCollisionBehavior(
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}}, {{20.0, 20.0, 18.0, 18.0, 16.0, 14.0, 12.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}},
        {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}}, {{20.0, 20.0, 20.0, 25.0, 25.0, 25.0}});

        std::function<franka::JointPositions(const franka::RobotState & robot_state, franka::Duration period)>
        jointMotionCallback = [&](const franka::RobotState &robot_state,
                                  franka::Duration period) -> franka::JointPositions {



            /*if (index == 0) {
                const auto initialPosition = robot_state.q;
                return (initialPosition);
            }*/
            index += period.toMSec();
            if (index >= desiredJointValuesMatrix.cols()) {
                index = desiredJointValuesMatrix.cols() -1;
            }
            Vector7d currentDesiredJointValues =  desiredJointValuesMatrix(all, index);
            std::array<double,7> jointValue{};
            std::copy_n(currentDesiredJointValues.data(), 7,jointValue.begin());

            if (index >= desiredJointValuesMatrix.cols()) {


                return franka::MotionFinished(franka::JointPositions(jointValue));

            }
            return (jointValue);

        };
        std::function<franka::Torques(const franka::RobotState &robot_state,franka::Duration period)> TorqueCallback = [&](const franka::RobotState &robot_state,franka::Duration period)->franka::Torques {


            controllerFranka controller({70,70,70,60,60,50,50},{30,30,30,20,20,10,10});
            Eigen::Map<const Eigen::Matrix<double,7,1>> desiredJointPositon(robot_state.q_d.data());
            return controller.torquePD(robot_state,desiredJointPositon,true,model.coriolis(robot_state));
            //return controller.advancedTorquePD(robot_state,desiredJointPositon,model.coriolis(robot_state),model.mass(robot_state));
        };
      realRobot.control(TorqueCallback,jointMotionCallback,true) ;



    } catch (const franka::Exception &ex) {
        std::cerr << ex.what() << std::endl;
        return -1;
    }


    return 0;
}

