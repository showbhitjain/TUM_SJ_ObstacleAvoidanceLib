//
// Created by shobhit on 08.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H


#include <string>
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <AndreiUtils/classes/ConfigurationParameters.hpp>

#include "Joints.h"
#include <tuple>
#include "Obstacle.h"
#include "CriticalPoints.h"
#include "robotLinkDataTypes.h"

namespace ObstacleAvoidance {


    class Robot {
    protected:
        AndreiUtils::Posed displacementEEToTCP; /**< The displacement from the end effector to the TCP. */
        Eigen::Matrix4d transformationEEToTCP;
        //the transformation between endeffector and TCP. (TCP w.r.t End effector frame)
        // TODO: remove below!
        AndreiUtils::Posed worldInBaseFrameRobot;
        AndreiUtils::ConfigurationParameters Config;
        std::shared_ptr<Joints> joints;
        Eigen::MatrixXd mdhMatrix;
        Eigen::VectorXd radiusLinks;
        Eigen::VectorXd radiusJoints;
        bool bConsiderBaseToFirstJoint;
        bool bConsiderRobotLinkAsLineSwept;
        std::string finalLinkType;
        Eigen::Vector3d finalLinkDimensions;
        std::vector<LinkSegment> robotLinks;
        FinalLinkRobot finalLinkGripper;
        bool bConsiderFinalLink;
        double distanceActivateOA;
        double distanceStopOA;
        double smootheningCoefficient;
        double distanceBuffer;
        bool doSelfCollisionAvoidance;
        double convergenceBeta;

        std::map<std::string,Obstacle> obstaclesDynamicMap;
        std::map<std::string,std::vector<CriticalPoints>> criticalPointsDynamicMap;
        // std::map<int, >

        [[nodiscard]] Eigen::MatrixXd
        jacobianCriticalPoint(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                              int const &indexLink) const;
        //      size_t number_joints;

        //get relative distance of the critical point to the link and the jacobian at critical Point
        [[nodiscard]] std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>
        criticalPointInformation(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                                 int const &indexLink) const;

        void
        deleteCriticalPoint(CriticalPoints &criticalPoint, bool const &bDeleteCriticalA, bool const &bDeleteCriticalD,
                            bool const &bDeleteCriticalFinalLink) const;

        [[nodiscard]] double computeB0(double const &bFirst,double const &distance) const;

        std::tuple<double,double> calculateDynamicSelfCollisionParameters(CriticalPoints &criticalPoint, std::vector<std::string> const &keySplit, bool const &enterCriticalZone, bool const &forFinalLink, bool const &forLinkA, bool const &forLinkD);

        void synchronizeMaps( std::map<std::string, Obstacle> const &obstaclesDynamicMap);
    public:
        Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot);

        Robot(AndreiUtils::ConfigurationParameters const & config);

        AndreiUtils::Posed getWorldInBaseFrameRobot() const;

        Eigen::VectorXd getRobotJointValues() const;


        Eigen::VectorXd getCurrentRobotJointValues() const;


        //void updateRobotJointValues();

        void setJointValues(Eigen::VectorXd const &jointValues) const;


        [[nodiscard]] int getNumberJoints() const;

        [[nodiscard]] const std::shared_ptr<Joints> &getJoints() const;


        [[nodiscard]] Eigen::MatrixXd jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const;


        [[nodiscard]] Eigen::MatrixXd jacobianCartesianOnLink(Eigen::VectorXd const &jointValues, int const &toIthLink,
                                                Eigen::Matrix4d const &transformationRelative) const;

        [[nodiscard]] Eigen::MatrixXd jacobianCartesianTCP(Eigen::VectorXd const &jointValues) const;

        [[nodiscard]] Eigen::MatrixXd forwardKinematicsTCP(Eigen::VectorXd const &jointValues) const;

        static Eigen::Matrix4d transformMdh(double const &a,  double const &alpha,  double const &d,  double  const &theta) ;

        [[nodiscard]] Eigen::MatrixXd fkmCartesian(const Eigen::VectorXd &joint_positions, int const &ith_link) const;

        [[nodiscard]] Eigen::MatrixXd fkmCartesianTCP(const Eigen::VectorXd &jointValues) const;


        std::pair<FinalLinkRobot, std::vector<LinkSegment>> createLinkSegments(const Eigen::VectorXd &jointValues);

        std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> obstacleAvoidanceEquation(
                std::map<std::string,Obstacle> const &obstaclesMap, Eigen::VectorXd const &jointAngles, Eigen::VectorXd const&jointVelocityOA);

        /*    std::vector<LinkSegment>
            createLinkSegments(const Eigen::VectorXd &jointValues, const Eigen::VectorXd &radius,
                               const std::string finalLinkType,
                               const Eigen::VectorXd &dimensions);*/
        void setObstacleMap(std::map<std::string,Obstacle> const & obstacleMap,bool saveLastSeenPosition = true);


        double getMinimumDistanceLinkAllObstacles(int const &linkIndex);

        Eigen::VectorXd minimumDistanceAllLinks();

        double getDistanceStopOA() const;

    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
