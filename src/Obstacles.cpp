//
// Created by shobhit on 30.11.24.
//

#include <Obstacles.h>
#include <DistanceObjects.h>


using namespace ObstacleAvoidance;
using namespace Eigen;


Obstacles::Obstacles(std::string const &obstacleTypeString, Eigen::Vector3d const &obstacleCenter,
                     Eigen::VectorXd const &obstacleDimensions,
                     Eigen::Vector4d const &obstacleOrientation, Eigen::Vector3d const &obstacleAxis) : obstacleType(
        obstacleTypeString),
                                                                                                        center(obstacleCenter),
                                                                                                        dimensions(
                                                                                                                obstacleDimensions),
                                                                                                        orientation(
                                                                                                                obstacleOrientation),
                                                                                                        axis(obstacleAxis.normalized()) {


}

Obstacles::Obstacles(ConceptLibrary::SphereShape const &sphereObstacle) {
    this->obstacleType = "Sphere";
    this->center = sphereObstacle.center;
    this->dimensions.resize(1);
    this->dimensions(0) = sphereObstacle.radius;
    this->axis = Eigen::Vector3d::Zero();
    this->orientation = {1, 0, 0, 0};
    this->obstacleName = sphereObstacle.obstacleName;
}


Obstacles::Obstacles(ConceptLibrary::CylinderShape const &cylinder) {
    this->obstacleType = "Cylinder";
    this->center = cylinder.center;
    this->dimensions.resize(2);
    this->dimensions[0] = cylinder.radius;
    this->dimensions[1] = cylinder.height;
    this->axis = cylinder.axis.normalized();
    this->obstacleName = cylinder.obstacleName;

}

Obstacles::Obstacles(ConceptLibrary::BoxShape const &box) {
    this->obstacleType = "Box";
    this->center = box.p;
    this->dimensions.resize(3);
    this->dimensions = box.dim;
    this->axis = Eigen::Vector3d::Zero();
    this->orientation = box.q;
    this->obstacleName = box.obstacleName;
}


Obstacles::Obstacles(Eigen::Vector3d const &vertexV0, Eigen::Vector3d const &vertexV1, double const &radius) {
    this->obstacleType = "Cylinder";
    this->dimensions.resize(2);
    this->dimensions[0] = radius;

    Vector3d axisVector = vertexV1 - vertexV0;
    //height
    this->dimensions[1] = (axisVector).norm();
    this->center = vertexV0 + (axisVector) * 0.5;
    this->axis = axisVector.normalized();

}

//distance, closest point Obstacle,closest point on final link
std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacles::calculateDistanceFinalLinkObstacle(finalLinkRobot const &linkEEtoTCP) const {
    if (linkEEtoTCP.type == "Box") {
        if (this->obstacleType == "Sphere") {
            return calculateDistanceSphereBox(this->dimensions[0], this->center, linkEEtoTCP.center,
                                              linkEEtoTCP.dimensions, linkEEtoTCP.orientation);
        }
        if (this->obstacleType == "Cylinder") {
            auto [distance, closestPointFinalLink, closestPointObstacle] = calculateDistanceBoxCylinder(
                    linkEEtoTCP.center, linkEEtoTCP.dimensions, linkEEtoTCP.orientation, this->dimensions[0],
                    this->dimensions[1], this->center, this->axis);
            return std::make_tuple(distance, closestPointObstacle, closestPointFinalLink);
        }
        if (this->obstacleType == "Box") {
            return calculateDistanceBoxBox(this->center, this->dimensions, this->orientation, linkEEtoTCP.center,
                                           linkEEtoTCP.dimensions, linkEEtoTCP.orientation);
        }
    }
    if (linkEEtoTCP.type == "Sphere") {
        if (this->obstacleType == "Sphere") {
            return calculateDistanceSphereSphere(this->dimensions[0], this->center, linkEEtoTCP.dimensions[0],
                                                 linkEEtoTCP.center);
        }
    } else {
        throw std::runtime_error("Either Invalid Obstacle Type or Invalid Final Link Type ");
    }

}


std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacles::calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                             Eigen::Vector3d const &linkSegmentV1,
                                                             double const &linkRadius) const {

    if (this->obstacleType == "Sphere") {
        Vector3d sphereCenter = this->center;
        double sphereRadius = this->dimensions(0);

        auto [distanceMagnitude, distanceVector, closestPointSphere, closestPointLinkSegment] = calculateDistanceSphereLineSweptSphere(
                sphereRadius, sphereCenter, linkSegmentV0, linkSegmentV1, linkRadius);

        return std::make_tuple(distanceMagnitude,closestPointSphere,closestPointLinkSegment);
        /*Vector3d axisSegment = linkSegmentV1 - linkSegmentV0;

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
        Vector3d closestPoint = linkSegmentV0 + (s * axisSegment);

        // Vector direction from sphere center to the closest point
        Vector3d distanceVector = closestPoint - sphereCenter;
        Vector3d distanceNormalized = distanceVector.normalized();
        double distanceMagnitude = distanceVector.norm() - sphereRadius - linkRadius;

        distanceVector = distanceMagnitude * distanceNormalized;

        Vector3d closestPointSphere = sphereCenter + (sphereRadius * distanceNormalized);
        Vector3d closestPointLinkSegment = closestPoint - (linkRadius * distanceNormalized);
        return std::make_tuple(distanceMagnitude, closestPointSphere, closestPointLinkSegment);*/

    }
}


std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacles::calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                              double const &radiusLink, double const &radiusJoint,
                                              bool const &robotLinkAsLineSwept) const {


    if (robotLinkAsLineSwept) {
        return this->calculateDistanceRobotLinkLineSweptObstacleSphere(startVertex, endVertex, radiusLink);
    } else if (!robotLinkAsLineSwept) {
        //Cylinder Part of link
        auto linkAxis = (endVertex - startVertex);
        double linkCylinderHeight = linkAxis.norm();
        Vector3d linkCylinderAxis = linkAxis.normalized();
        Vector3d linkCylinderCenter = startVertex + (linkCylinderAxis * (linkCylinderHeight / 2.0));
        double linkCylinderRadius = radiusLink;

        //Joint Part of link
        double linkSphereRadius = radiusJoint;
        Vector3d linkSphereCenter = startVertex;

        Vector3d closestPointLink;
        Vector3d closestPointObstacle;
        double distance;

        if (this->obstacleType == "Sphere") {
            auto sphereCenter = this->center;
            auto sphereRadius = this->dimensions(0);


            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] = calculateDistanceSphereCylinder(
                    sphereRadius, sphereCenter, linkCylinderRadius,
                    linkCylinderHeight, linkCylinderCenter, linkCylinderAxis);
            //        std::cout<<"distance between sphere and cylinder link: "<<distanceCylinderObstacle<<std::endl;

            auto [distanceSphereObstacle, closestPointObstacleFromSphere, closestPointLinkSphere] = calculateDistanceSphereSphere(
                    sphereRadius, sphereCenter, linkSphereRadius, linkSphereCenter);
            //        std::cout<<"distance between sphere and sphere joint: "<<distanceSphereObstacle<<std::endl;


            if (distanceCylinderObstacle < distanceSphereObstacle) {
                distance = distanceCylinderObstacle;
                closestPointLink = closestPointLinkCylinder;
                closestPointObstacle = closestPointObstacleFromCylinder;
            } else {
                distance = distanceSphereObstacle;
                closestPointLink = closestPointLinkSphere;
                closestPointObstacle = closestPointObstacleFromSphere;
            }
            return std::make_tuple(distance, closestPointObstacle, closestPointLink);
        }
        //        calculateDistanceSphereCylinder(startVertex,radiusJoint,)

        if (this->obstacleType == "Cylinder") {

            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] = calculateDistanceCylinderCylinder(
                    this->dimensions[0], this->dimensions[1],
                    this->center, this->axis, linkCylinderRadius,
                    linkCylinderHeight, linkCylinderCenter,
                    linkCylinderAxis);
            auto [distanceSphereObstacle, closestPointLinkSphere, closestPointObstacleFromSphere] = calculateDistanceSphereCylinder(
                    linkSphereRadius, linkSphereCenter, linkCylinderRadius,
                    linkCylinderHeight, linkCylinderCenter,
                    linkCylinderAxis);

            if (distanceCylinderObstacle < distanceSphereObstacle) {

                distance = distanceCylinderObstacle;
                closestPointLink = closestPointLinkCylinder;
                closestPointObstacle = closestPointObstacleFromCylinder;
            } else {
                distance = distanceSphereObstacle;
                closestPointLink = closestPointLinkSphere;
                closestPointObstacle = closestPointObstacleFromSphere;
            }
            return std::make_tuple(distance, closestPointObstacle, closestPointLink);
        }

        if (this->obstacleType == "Box") {
            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] = calculateDistanceBoxCylinder(
                    this->center, this->dimensions, this->orientation,
                    linkCylinderRadius, linkCylinderHeight, linkCylinderCenter,
                    linkCylinderAxis);

            auto [distanceSphereObstacle, closestPointLinkSphere, closestPointObstacleFromSphere] = calculateDistanceSphereBox(
                    linkSphereRadius, linkSphereCenter, this->center, this->dimensions, this->orientation);

            if (distanceCylinderObstacle < distanceSphereObstacle) {

                distance = distanceCylinderObstacle;
                closestPointLink = closestPointLinkCylinder;
                closestPointObstacle = closestPointObstacleFromCylinder;
            } else {
                distance = distanceSphereObstacle;
                closestPointLink = closestPointLinkSphere;
                closestPointObstacle = closestPointObstacleFromSphere;
            }
            return std::make_tuple(distance, closestPointObstacle, closestPointLink);
        }
    }
}



