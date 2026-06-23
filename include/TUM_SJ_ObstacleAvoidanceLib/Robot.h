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
        Eigen::Matrix4d transformationEEToTCP; /**< Transformation of the TCP expressed in the end-effector frame. */
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

        /**
         * @brief Compute the Cartesian Jacobian of a point on a link relative to that link's frame.
         * @param jointAngles Current joint angles.
         * @param closestPointLink Point (in base frame) lying on the link of interest.
         * @param indexLink Index of the link the point belongs to.
         * @return 6xN Jacobian of the given point.
         */
        [[nodiscard]] Eigen::MatrixXd
        jacobianCriticalPoint(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                              int const &indexLink) const;

        /**
         * @brief Compute the relative pose of a critical point and the Jacobian at that point.
         * @param jointAngles Current joint angles.
         * @param closestPointLink Critical point (in base frame) on the link.
         * @param indexLink Index of the link the point belongs to.
         * @return Tuple of (relative transformation of the point w.r.t. the link frame, Jacobian at the point).
         */
        [[nodiscard]] std::tuple<Eigen::MatrixXd, Eigen::MatrixXd>
        criticalPointInformation(Eigen::VectorXd const &jointAngles, Eigen::Vector3d const &closestPointLink,
                                 int const &indexLink) const;

        /**
         * @brief Reset (delete) the selected critical-point channels of a CriticalPoints entry to their default state.
         * @param criticalPoint Critical-point entry to modify.
         * @param bDeleteCriticalA Whether to reset the A-segment critical point.
         * @param bDeleteCriticalD Whether to reset the D-segment critical point.
         * @param bDeleteCriticalFinalLink Whether to reset the final-link critical point.
         */
        void
        deleteCriticalPoint(CriticalPoints &criticalPoint, bool const &bDeleteCriticalA, bool const &bDeleteCriticalD,
                            bool const &bDeleteCriticalFinalLink) const;

        /**
         * @brief Apply the smoothing constraint scheme to a constraint bound based on the distance.
         * @param bFirst Raw constraint bound value.
         * @param distance Current distance used to scale the bound.
         * @return Smoothed constraint bound.
         */
        [[nodiscard]] double computeB0(double const &bFirst,double const &distance) const;

        /**
         * @brief Compute the dynamic self-collision constraint bound and blending factor lambda for a critical point.
         * @param criticalPoint Critical-point entry being processed.
         * @param keySplit Obstacle key split on "_" (identifies self-collision link obstacles).
         * @param enterCriticalZone Whether the link is entering the critical zone in this step.
         * @param forFinalLink Whether the computation targets the final link.
         * @param forLinkA Whether the computation targets the A segment.
         * @param forLinkD Whether the computation targets the D segment.
         * @return Tuple of (dynamic constraint bound bSecond, blending factor lambda).
         */
        std::tuple<double,double> calculateDynamicSelfCollisionParameters(CriticalPoints &criticalPoint, std::vector<std::string> const &keySplit, bool const &enterCriticalZone, bool const &forFinalLink, bool const &forLinkA, bool const &forLinkD);

        /**
         * @brief Synchronize the critical-points map with the current obstacle map (add/remove entries).
         * @param obstaclesDynamicMap Current obstacle map whose keys define the valid critical-point entries.
         */
        void synchronizeMaps( std::map<std::string, Obstacle> const &obstaclesDynamicMap);
    public:
        /**
         * @brief Construct a Robot from a configuration file.
         * @param configFile_Path Path to the file containing the configuration paths/parameters.
         * @param parameterFor Top-level configuration section to use.
         * @param whichrobot Sub-configuration name identifying the robot type.
         */
        Robot(const std::string &configFile_Path, const std::string &parameterFor, const std::string &whichrobot);

        /**
         * @brief Construct a Robot directly from configuration parameters.
         * @param config Configuration parameters describing the robot model and obstacle-avoidance settings.
         */
        Robot(AndreiUtils::ConfigurationParameters const & config);

        /**
         * @brief Get the pose of the world frame expressed in the robot base frame.
         * @return World-in-base pose.
         */
        AndreiUtils::Posed getWorldInBaseFrameRobot() const;

        /**
         * @brief Get the last-known joint values without updating them.
         * @return Joint values vector.
         */
        Eigen::VectorXd getRobotJointValues() const;

        /**
         * @brief Get the current joint values, refreshing them at call time.
         * @return Joint values vector.
         */
        Eigen::VectorXd getCurrentRobotJointValues() const;

        /**
         * @brief Set the robot joint values.
         * @param jointValues New joint values.
         */
        void setJointValues(Eigen::VectorXd const &jointValues) const;

        /**
         * @brief Get the number of joints of the robot.
         * @return Number of joints.
         */
        [[nodiscard]] int getNumberJoints() const;

        /**
         * @brief Access the underlying Joints object.
         * @return Shared pointer to the Joints object.
         */
        [[nodiscard]] const std::shared_ptr<Joints> &getJoints() const;

        /**
         * @brief Compute the geometric Cartesian Jacobian up to the given link.
         * @param jointValues Current joint values.
         * @param toIthLink Link index up to which the Jacobian is computed.
         * @return 6xN Jacobian (linear velocity in rows 0-2, angular velocity in rows 3-5).
         */
        [[nodiscard]] Eigen::MatrixXd jacobianCartesian(const Eigen::VectorXd &jointValues, const int &toIthLink) const;

        /**
         * @brief Compute the Cartesian Jacobian of a point on a link given its relative transformation.
         * @param jointValues Current joint values.
         * @param toIthLink Link index the point belongs to.
         * @param transformationRelative Transformation of the point relative to the link frame.
         * @return 6xN Jacobian of the point.
         */
        [[nodiscard]] Eigen::MatrixXd jacobianCartesianOnLink(Eigen::VectorXd const &jointValues, int const &toIthLink,
                                                Eigen::Matrix4d const &transformationRelative) const;

        /**
         * @brief Compute the Cartesian Jacobian at the TCP.
         * @param jointValues Current joint values.
         * @return 6xN Jacobian at the TCP.
         */
        [[nodiscard]] Eigen::MatrixXd jacobianCartesianTCP(Eigen::VectorXd const &jointValues) const;

        /**
         * @brief Build the homogeneous transformation for a single modified Denavit-Hartenberg (MDH) link.
         * @param a Link length a.
         * @param alpha Link twist alpha (radians).
         * @param d Link offset d.
         * @param theta Joint angle theta (radians).
         * @return 4x4 MDH transformation matrix.
         */
        static Eigen::Matrix4d transformMdh(double const &a,  double const &alpha,  double const &d,  double  const &theta) ;

        /**
         * @brief Forward kinematics: transformation from the base frame up to the given link.
         * @param joint_positions Joint positions.
         * @param ith_link Link index up to which forward kinematics is computed.
         * @return 4x4 homogeneous transformation of the ith link in the base frame.
         */
        [[nodiscard]] Eigen::MatrixXd fkmCartesian(const Eigen::VectorXd &joint_positions, int const &ith_link) const;

        /**
         * @brief Forward kinematics to the TCP.
         * @param jointValues Joint values.
         * @return 4x4 homogeneous transformation of the TCP in the base frame.
         */
        [[nodiscard]] Eigen::MatrixXd fkmCartesianTCP(const Eigen::VectorXd &jointValues) const;

        /**
         * @brief Build the geometric link segments (A/D segments) and final-link representation of the robot.
         * @param jointValues Current joint values.
         * @return Pair of (final-link representation, vector of per-link segments).
         */
        std::pair<FinalLinkRobot, std::vector<LinkSegment>> createLinkSegments(const Eigen::VectorXd &jointValues);

        /**
         * @brief Assemble the obstacle-avoidance inequality constraints for the current configuration.
         * @param obstaclesMap Map of obstacles (and, when enabled, self-collision link obstacles) to consider.
         * @param jointAngles Current joint angles.
         * @param jointVelocityOA Joint velocity from the previous obstacle-avoidance step.
         * @return Tuple of (constraint Jacobian j0, constraint bound vector b0, minimum distance to any obstacle).
         */
        std::tuple<Eigen::MatrixXd, Eigen::VectorXd, double> obstacleAvoidanceEquation(
                std::map<std::string,Obstacle> const &obstaclesMap, Eigen::VectorXd const &jointAngles, Eigen::VectorXd const&jointVelocityOA);

        /**
         * @brief Update the internal obstacle map.
         * @param obstacleMap New obstacle map.
         * @param saveLastSeenObstacle If true, obstacles missing from @p obstacleMap are kept (last-seen); otherwise removed.
         */
        void setObstacleMap(std::map<std::string,Obstacle> const & obstacleMap,bool saveLastSeenObstacle = true);

        /**
         * @brief Get the minimum distance between a given link and all obstacles (for plotting/analysis).
         * @param linkIndex Link index (must be valid for the robot).
         * @return Minimum distance, or NaN if no valid obstacle distance is available.
         */
        double getMinimumDistanceLinkAllObstacles(int const &linkIndex);

        /**
         * @brief Get the minimum distance to obstacles for every link.
         * @return Vector of per-link minimum distances.
         */
        Eigen::VectorXd minimumDistanceAllLinks();

        /**
         * @brief Get the configured stop distance for obstacle avoidance.
         * @return Stop distance threshold d1.
         */
        double getDistanceStopOA() const;

    };
}

#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_ROBOT_H
