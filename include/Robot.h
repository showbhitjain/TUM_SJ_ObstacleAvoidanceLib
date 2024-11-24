//
// Created by shobhit on 08.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H

#include <dqrobotics/robot_modeling/DQ_SerialManipulatorMDH.h>
#include <dqrobotics/robot_modeling/DQ_SerialManipulatorDH.h>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <Joints.h>

namespace ObstacleAvoidance {
    class Robot {
    protected:
        static DQ_robotics::DQ_SerialManipulator
        createRobot_fromconfigfile(const std::string &configFile, const DQ_robotics::DQ &q);

        std::shared_ptr<DQ_robotics::DQ_SerialManipulatorDH> robotDH;
        std::shared_ptr<DQ_robotics::DQ_SerialManipulatorMDH> robot;

        DQ_robotics::DQ_SerialManipulatorMDH robotmdh;
        AndreiUtils::Posed displacementEEToTCP; /**< The displacement from the end effector to the TCP. */
        Eigen::Matrix4d transformationEEToTCP; //the transformation between endeffector and TCP. (TCP w.r.t End effector frame)
        AndreiUtils::Posed baseFrame_robot;
        AndreiUtils::ConfigurationParameters Config;
        std::shared_ptr<Joints> joints;
//        size_t number_joints;

    public:
        Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot,
              const AndreiUtils::Posed &baseFrame);

        DQ_robotics::DQ_SerialManipulatorMDH
        createRobotMDH(const AndreiUtils::ConfigurationParameters &config, const DQ_robotics::DQ &q);

        Eigen::VectorXd getRobotJointValues() const;

        DQ_robotics::DQ getRobotPose() const;

        Eigen::VectorXd getCurrentRobotJointValues();

        DQ_robotics::DQ getCurrentRobotPose();

        //void updateRobotJointValues();

        void setJointValues(Eigen::VectorXd const &jointValues);

        DQ_robotics::DQ fkm() const;

        DQ_robotics::DQ fkm(Eigen::VectorXd const &jointValues) const;

        DQ_robotics::DQ fkm_toith(Eigen::VectorXd const &jointValues) const;

        Eigen::MatrixXd jacobian() const;

        Eigen::MatrixXd jacobian(Eigen::VectorXd const &jointValues) const;

        Eigen::VectorXd fkm_cartesian(Eigen::VectorXd const &jointValues) const;

        Eigen::VectorXd fkm_cartesian(Eigen::VectorXd const &jointValues, const int &to_ith_link) const;

        Eigen::MatrixXd forwardKinematics(const Eigen::VectorXd &jointValues, const int &toIthLink) const;

        size_t getNumberJoints() const;

        const std::shared_ptr<Joints> &getJoints() const;

        DQ_robotics::DQ_SerialManipulatorMDH
        createRobotMdhFromPath(const std::string &configFile_Path, const std::string &parameterFor,
                               const std::string &whichrobot, const AndreiUtils::Posed &baseFrame);

        Eigen::MatrixXd jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const;



        Eigen::MatrixXd jacobianCartesianOnLink( Eigen::VectorXd const &jointValues,  int const &toIthLink,
                                                 Eigen::Matrix4d const &transformationRelative);

        Eigen::MatrixXd jacobianCartesianTCP(Eigen::VectorXd const &jointValues);

        Eigen::MatrixXd forwardKinematicsTCP( Eigen::VectorXd const &jointValues) const;
    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
