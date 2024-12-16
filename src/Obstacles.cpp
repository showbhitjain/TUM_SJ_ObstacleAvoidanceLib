//
// Created by shobhit on 30.11.24.
//

#include <Obstacles.h>

using namespace ObstacleAvoidance;
using namespace Eigen;


Obstacles::Obstacles(const std::string &obstacleType, const Eigen::VectorXd &center, const Eigen::VectorXd &dimensions,
                      Eigen::Vector4d orientation,  Eigen::Vector3d axis) : obstacleType(obstacleType),
                                                                                        Center(center),
                                                                                        dimensions(dimensions),
                                                                                        orientation(orientation),
                                                                                        axis(axis) {



}
Obstacles::Obstacles(ConceptLibrary::SphereShape const & sphereObstacle){
    this->Center = sphereObstacle.center;
    this->dimensions.resize(1);
    this->dimensions(0) = sphereObstacle.radius;
    this->axis = Eigen::Vector3d::Zero();
    this->orientation = {1,0,0,0};
}


std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
Obstacles::calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &linkSegmentV0, Eigen::Vector3d const &linkSegmentV1,
                                              double const &linkRadius) {
    if (this->obstacleType == "Sphere")
    {
        auto sphereCenter  = this->Center;
        auto sphereRadius = this->dimensions(0);

        Vector3d axisSegment = linkSegmentV1 - linkSegmentV0;

        // Vector from LSS start vertex to sphere center
        Eigen::Vector3d linkSegmentV0ToSphereCenter = sphereCenter - linkSegmentV0;

        // Projection of linkSegmentV0ToSphereCenter on the line segment
        double projection = linkSegmentV0ToSphereCenter.dot(axisSegment);

        // Parameter s along the line segment
        double s;
        if (projection <= 0) {
            s = 0;
        } else if (projection >= axisSegment.squaredNorm()) {
            s = 1;
        } else {
            s = projection / axisSegment.squaredNorm();
        }

        // Calculate the point on LSS closest to the sphere center
        Vector3d closestPoint = linkSegmentV0 + s * axisSegment;

        // Vector direction from sphere center to the closest point
        Vector3d distanceVector = closestPoint - sphereCenter;
        Vector3d distanceNormalized = distanceVector.normalized();
        double distanceMagnitude = distanceVector.norm() - sphereRadius - linkRadius;

        distanceVector = distanceMagnitude * distanceNormalized;

        Vector3d closestPointSphere = sphereCenter + sphereRadius * distanceNormalized;
        Vector3d closestPointLinkSegment = closestPoint - linkRadius * distanceNormalized;
        // std::make_tuple()
    }

    return std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>();
}
