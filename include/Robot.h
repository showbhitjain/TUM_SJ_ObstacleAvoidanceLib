//
// Created by shobhit on 08.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H

//#include <dqrobotics/robot_modeling/DQ_SerialManipulatorMDH.h>
#include <dqrobotics/robot_modeling/DQ_SerialManipulatorDH.h>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <Joints.h>
#include <tuple>
#include <Obstacles.h>
#include <LinkSegment.h>

namespace ObstacleAvoidance {

    class Robot {
    protected:
        AndreiUtils::Posed displacementEEToTCP; /**< The displacement from the end effector to the TCP. */
        Eigen::Matrix4d transformationEEToTCP;
        //the transformation between endeffector and TCP. (TCP w.r.t End effector frame)
        AndreiUtils::Posed baseFrame_robot;
        AndreiUtils::ConfigurationParameters Config;
        std::shared_ptr<Joints> joints;
        Eigen::MatrixXd mdhMatrix;
        Eigen::VectorXd radiusLinks;
        Eigen::VectorXd radiusJoints;
        bool splitRegion;
        bool splitRegionFinalLink;
        std::string finalLinkType;
        Eigen::Vector3d finalLinkDimensions;

        //      size_t number_joints;


    public:
        Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot);


        Eigen::VectorXd getRobotJointValues() const;


        Eigen::VectorXd getCurrentRobotJointValues();


        //void updateRobotJointValues();

        void setJointValues(Eigen::VectorXd const &jointValues);


        [[nodiscard]] int getNumberJoints() const;

        [[nodiscard]] const std::shared_ptr<Joints> &getJoints() const;


        Eigen::MatrixXd jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const;


        Eigen::MatrixXd jacobianCartesianOnLink(Eigen::VectorXd const &jointValues, int const &toIthLink,
                                                Eigen::Matrix4d const &transformationRelative);

        Eigen::MatrixXd jacobianCartesianTCP(Eigen::VectorXd const &jointValues);

        Eigen::MatrixXd forwardKinematicsTCP(Eigen::VectorXd const &jointValues) const;

        Eigen::Matrix4d transformMdh(double a, double alpha, double d, double theta) const;

        Eigen::MatrixXd fkmCartesian(const Eigen::VectorXd &joint_positions, int const &ith_link) const;

        Eigen::MatrixXd fkmCartesianTCP(const Eigen::VectorXd &jointValues) const;


        std::vector<LinkSegment> createLineSegments(const Eigen::VectorXd &jointValues);

        std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> obstacleAvoidanceEquation(
                std::vector<Obstacles> obstacles, Eigen::VectorXd jointAngles, Eigen::VectorXd const &radiusLinks,
                double distOuter, double distStop, double k);

        std::vector<LinkSegment>
        createLineSegments(const Eigen::VectorXd &jointValues, const Eigen::VectorXd &radius,
                           const std::string finalLinkType,
                           const Eigen::VectorXd &dimensions);
    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
