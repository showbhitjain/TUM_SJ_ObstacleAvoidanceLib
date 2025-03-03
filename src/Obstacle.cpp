//
// Created by shobhit on 30.11.24.
//

#include <TUM_SJ_ObstacleAvoidanceLib/Obstacle.h>
#include <TUM_SJ_ObstacleAvoidanceLib/DistanceObjects.h>


using namespace ObstacleAvoidance;
using namespace Eigen;


Obstacle::Obstacle(std::string const &obstacleTypeString, Eigen::Vector3d const &obstacleCenter,
                   Eigen::VectorXd const &obstacleDimensions,
                   Eigen::Vector4d const &obstacleOrientation, Eigen::Vector3d const &obstacleAxis) : obstacleType(
        obstacleTypeString), center(obstacleCenter), dimensions(obstacleDimensions), orientation(obstacleOrientation),
                                                                                                      axis(obstacleAxis.normalized()) {}

Obstacle::Obstacle(ConceptLibrary::SphereShape const &sphereObstacle) {
    this->obstacleType = "Sphere";
    this->center = sphereObstacle.center;
    this->dimensions.resize(1);
    this->dimensions(0) = sphereObstacle.radius;
    this->axis = Eigen::Vector3d::Zero();
    this->orientation = {1, 0, 0, 0};
    this->obstacleName = sphereObstacle.obstacleName;
}


Obstacle::Obstacle(ConceptLibrary::CylinderShape const &cylinder) {
    this->obstacleType = "Cylinder";
    this->center = cylinder.center;
    this->dimensions.resize(2);
    this->dimensions[0] = cylinder.radius;
    this->dimensions[1] = cylinder.height;
    this->axis = cylinder.axis.normalized();
    this->obstacleName = cylinder.obstacleName;
}

Obstacle::Obstacle(ConceptLibrary::BoxShape const &box) {
    this->obstacleType = "Box";
    this->center = box.p;
    this->dimensions.resize(3);
    this->dimensions = box.dim;
    this->axis = Eigen::Vector3d::Zero();
    this->orientation = box.q;
    this->obstacleName = box.obstacleName;
}

Obstacle::Obstacle(ConceptLibrary::PlaneWidthShape const &planeShapeWithWidth) {
    this->obstacleType = "Box";
    double boxWidth = planeShapeWithWidth.width;
    Vector3d axisNormalized = planeShapeWithWidth.axis.normalized();
    this->center = planeShapeWithWidth.point - ((boxWidth/2) * axisNormalized);
    this->dimensions.resize(3);

    //set a huge value in order to depict infinite dimension
    this->dimensions[0] = 100;
    this->dimensions[1] = 100;
    this->dimensions[2] = boxWidth;
    Eigen::Quaterniond orientationPlane = Eigen::Quaterniond::FromTwoVectors(
            Eigen::Vector3d::UnitZ(), axisNormalized
    ).normalized();
    this->orientation = orientationPlane;
    // this->center =
    this->axis = planeShapeWithWidth.axis;
    this->obstacleName = planeShapeWithWidth.obstacleName;
}


Obstacle::Obstacle(Eigen::Vector3d const &vertexV0, Eigen::Vector3d const &vertexV1, double const &radius) {
    this->obstacleType = "Cylinder";
    this->dimensions.resize(2);
    this->dimensions[0] = radius;
    auto linkAxis = (vertexV1 - vertexV0);
    double linkCylinderHeight = linkAxis.norm();
    Vector3d linkCylinderAxis = linkAxis.normalized();
    Vector3d linkCylinderCenter = vertexV0 + (linkCylinderAxis * (linkCylinderHeight / 2.0));

    //height
    this->dimensions[1] = linkCylinderHeight;

    this->center = linkCylinderCenter;
    this->axis = linkCylinderAxis;
}


//distance, closest point Obstacle,closest point on final link
std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacle::calculateDistanceFinalLinkObstacle(FinalLinkRobot const &linkEEtoTCP, bool const &usePartialBoxLink) const {
    if (linkEEtoTCP.type == "Box") {
        Vector3d linkBoxCenter = linkEEtoTCP.center;
        Vector3d linkBoxDimensions = linkEEtoTCP.dimensions;
        Eigen::Quaterniond linkBoxOrientation = linkEEtoTCP.orientation;
        Matrix3d linkBoxRotationMatrix = linkBoxOrientation.toRotationMatrix();
        if (usePartialBoxLink) {
            if (linkEEtoTCP.directionPartialFinalLink != "X" &&
                linkEEtoTCP.directionPartialFinalLink != "Y" &&
                linkEEtoTCP.directionPartialFinalLink != "Z") {
                std::cerr << "Error: Invalid directionPartialFinalLink: "
                          << linkEEtoTCP.directionPartialFinalLink << std::endl;
                assert(false); // This will still terminate the program
            }

            if (linkEEtoTCP.directionPartialFinalLink == "Z") {
                linkBoxCenter = linkBoxCenter + (linkBoxRotationMatrix(all, 2) * linkBoxDimensions[2] / 4);
                linkBoxDimensions[2] = linkBoxDimensions[2] / 2;
            } else if (linkEEtoTCP.directionPartialFinalLink == "Y") {
                linkBoxCenter = linkBoxCenter + (linkBoxRotationMatrix(all, 1) * linkBoxDimensions[1] / 4);
                linkBoxDimensions[1] = linkBoxDimensions[1] / 2;
            } else if (linkEEtoTCP.directionPartialFinalLink == "X") {
                linkBoxCenter = linkBoxCenter + (linkBoxRotationMatrix(all, 0) * linkBoxDimensions[0] / 4);
                linkBoxDimensions[0] = linkBoxDimensions[0] / 2;
            }
        }

        if (this->obstacleType == "Sphere") {
            return calculateDistanceSphereBox(this->dimensions[0], this->center, linkBoxCenter,
                                              linkBoxDimensions, linkBoxOrientation);
        }
        if (this->obstacleType == "Cylinder") {
            auto [distance, closestPointFinalLink, closestPointObstacle] = calculateDistanceBoxCylinder(
                    linkBoxCenter, linkBoxDimensions, linkBoxOrientation, this->dimensions[0],
                    this->dimensions[1], this->center, this->axis);
            return std::make_tuple(distance, closestPointObstacle, closestPointFinalLink);
        }
        if (this->obstacleType == "Box") {
            return calculateDistanceBoxBox(this->center, this->dimensions, this->orientation, linkBoxCenter,
                                           linkBoxDimensions, linkBoxOrientation);
        }
    }
    if (linkEEtoTCP.type == "Sphere") {
        if (this->obstacleType == "Sphere") {
            return calculateDistanceSphereSphere(this->dimensions[0], this->center, linkEEtoTCP.dimensions[0],
                                                 linkEEtoTCP.center);
        }
    }
    throw std::runtime_error("Either Invalid Obstacle Type or Invalid Final Link Type");
}


std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacle::calculateDistanceRobotLinkLineSweptObstacleSphere(Eigen::Vector3d const &linkSegmentV0,
                                                            Eigen::Vector3d const &linkSegmentV1,
                                                            double const &linkRadius) const {
    if (this->obstacleType == "Sphere") {
        Vector3d sphereCenter = this->center;
        double sphereRadius = this->dimensions(0);

        auto [distanceMagnitude, distanceVector, closestPointSphere, closestPointLinkSegment] =
                calculateDistanceSphereLineSweptSphere(
                        sphereRadius, sphereCenter, linkSegmentV0, linkSegmentV1, linkRadius);

        return std::make_tuple(distanceMagnitude, closestPointSphere, closestPointLinkSegment);
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

    throw std::runtime_error("Can not use obstacle type: " + this->obstacleType +
                             " in calculateDistanceRobotLinkLineSweptObstacleSphere");
}


std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
Obstacle::calculateDistanceRobotLinkObstacle(Eigen::Vector3d const &startVertex, Eigen::Vector3d const &endVertex,
                                             double const &radiusLink, double const &radiusJoint,
                                             bool const &robotLinkAsLineSwept, bool const &usePartialLink) const {
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


        //Use second half of  Link
        if (usePartialLink) {
            linkCylinderHeight = linkCylinderHeight / 2.0;
            linkCylinderCenter = startVertex + (linkCylinderAxis * (3.0 * linkCylinderHeight / 2.0));

            linkSphereCenter = startVertex + (linkCylinderAxis * linkCylinderHeight);
        }

        Vector3d closestPointLink;
        Vector3d closestPointObstacle;
        double distance;

        if (this->obstacleType == "Sphere") {
            auto sphereCenter = this->center;
            auto sphereRadius = this->dimensions(0);


            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] =
                    calculateDistanceSphereCylinder(
                            sphereRadius, sphereCenter, linkCylinderRadius,
                            linkCylinderHeight, linkCylinderCenter, linkCylinderAxis);
            //        std::cout<<"distance between sphere and cylinder link: "<<distanceCylinderObstacle<<std::endl;

            auto [distanceSphereObstacle, closestPointObstacleFromSphere, closestPointLinkSphere] =
                    calculateDistanceSphereSphere(
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

        else if (this->obstacleType == "Cylinder") {
            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] =
                    calculateDistanceCylinderCylinder(
                            this->dimensions[0], this->dimensions[1],
                            this->center, this->axis, linkCylinderRadius,
                            linkCylinderHeight, linkCylinderCenter,
                            linkCylinderAxis);
            auto [distanceSphereObstacle, closestPointLinkSphere, closestPointObstacleFromSphere] =
                    calculateDistanceSphereCylinder(
                            linkSphereRadius, linkSphereCenter, this->dimensions[0],
                            this->dimensions[1], this->center,
                            this->axis);

            if (distanceCylinderObstacle < distanceSphereObstacle) {
                distance = distanceCylinderObstacle;
                closestPointLink = closestPointLinkCylinder;
                closestPointObstacle = closestPointObstacleFromCylinder;
            } else {
                distance = distanceSphereObstacle;
                closestPointLink = closestPointLinkSphere;
                closestPointObstacle = closestPointObstacleFromSphere;
            }
            if (usePartialLink) {
                return std::make_tuple(distanceCylinderObstacle, closestPointObstacleFromCylinder,
                                       closestPointLinkCylinder);
            } else {
                return std::make_tuple(distance, closestPointObstacle, closestPointLink);
            }
        } else if (this->obstacleType == "Box") {
            auto [distanceCylinderObstacle, closestPointObstacleFromCylinder, closestPointLinkCylinder] =
                    calculateDistanceBoxCylinder(
                            this->center, this->dimensions, this->orientation,
                            linkCylinderRadius, linkCylinderHeight, linkCylinderCenter,
                            linkCylinderAxis);

            auto [distanceSphereObstacle, closestPointLinkSphere, closestPointObstacleFromSphere] =
                    calculateDistanceSphereBox(
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

    throw std::runtime_error("Unknown obstacle type: " + this->obstacleType);
}
