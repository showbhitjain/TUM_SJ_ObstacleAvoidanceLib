//
// Created by shobhit on 26.12.24.
//

#include <TUM_SJ_ObstacleAvoidanceLib/DistanceObjects.h>
#include <fcl/fcl.h>


using namespace Eigen;
using namespace std;


namespace ObstacleAvoidance {
//calculate closest points and distance between two spheres
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereSphere(double const &radius1, Eigen::Vector3d const &center1, double const &radius2,
                                  Eigen::Vector3d const &center2) {

        Vector3d distanceVector = center1 - center2;
        auto distanceNormalized = distanceVector.normalized();
        double distanceMagnitude = distanceVector.norm() - radius1 - radius2;
        Eigen::Vector3d closestPointSphere1 = center1 - radius1 * distanceNormalized;
        Eigen::Vector3d closestPointSphere2 = center2 + radius2 * distanceNormalized;
        return std::make_tuple(distanceMagnitude, closestPointSphere1, closestPointSphere2);

    }


/*@param radiusSphere    Radius of the sphere
* @param sphereCenter    center of the sphere
* @param radiusCylinder  Radius of the cylinder
* @param cylinderHeight  Total height (length) of the cylinder
* @param cylinderCenter  center of the cylinder (midpoint along its axis)
* @param cylinderAxis    Normalized direction vector of cylinder's axis*/
//calculate closest points and distance between sphere and Cylinder
//return sequence: distance magnitude, sphere closest point, cylinder closest point
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereCylinder(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                                    double const &radiusCylinder, double const &cylinderHeight,
                                    Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis) {

        //If not already normalised, then normalise
        Vector3d axisNorm = cylinderAxis.normalized();

        Vector3d cylinderStartVertex = cylinderCenter + axisNorm * (-cylinderHeight / 2);
        Vector3d cylinderEndVertex = cylinderCenter + axisNorm * (cylinderHeight / 2);
        Vector3d axisSegment = cylinderEndVertex - cylinderStartVertex;

        // Vector from LSS start vertex to sphere center
        Eigen::Vector3d cylinderStartToSphereCenter = sphereCenter - cylinderStartVertex;
        Vector3d closestPointSphere;
        Vector3d closestPointCylinder;
        Vector3d closestPointCylinderAxis;
        Vector3d distanceVector;
        double projection = cylinderStartToSphereCenter.dot(axisSegment);
        double s;
        int distanceIntersectionDirection = 1;
        if (projection <= 0) {
            //closest point on Cylinder lies on the circle with starting vertex of axis as center
            s = 0;
            closestPointCylinderAxis = cylinderStartVertex;
            distanceVector = sphereCenter - closestPointCylinderAxis;
            double distanceVectorNormalProjectedMagnitude = distanceVector.dot(-axisNorm);
            auto distanceVectorNormalProjected = distanceVectorNormalProjectedMagnitude * (-axisNorm);
            auto projectedVector = sphereCenter - distanceVectorNormalProjected;
            auto vectorClosestPointCylinder = projectedVector - cylinderStartVertex;
            if (vectorClosestPointCylinder.norm() >= radiusCylinder) {
                closestPointCylinder = cylinderStartVertex + (vectorClosestPointCylinder.normalized() * radiusCylinder);
            } else {
                closestPointCylinder = cylinderStartVertex + vectorClosestPointCylinder;
            }
            distanceVector = sphereCenter - closestPointCylinder;
            if (distanceVector.norm() < radiusSphere) { distanceIntersectionDirection = -1; }
            closestPointSphere = sphereCenter - (distanceVector.normalized() * radiusSphere);
        } else if (projection >= axisSegment.squaredNorm()) {
            //closest point on Cylinder lies on the circle with end vertix of axis as center
            s = 1;
            closestPointCylinderAxis = cylinderEndVertex;
            distanceVector = sphereCenter - closestPointCylinderAxis;
            double distanceVectorNormalProjectedMagnitude = distanceVector.dot(axisNorm);
            auto distanceVectorNormalProjected = distanceVectorNormalProjectedMagnitude * axisNorm;
            auto projectedVector = sphereCenter - distanceVectorNormalProjected;
            auto vectorClosestPointCylinder = projectedVector - cylinderEndVertex;
            if (vectorClosestPointCylinder.norm() >= radiusCylinder) {
                closestPointCylinder = cylinderEndVertex + (vectorClosestPointCylinder.normalized() * radiusCylinder);
            } else {
                closestPointCylinder = cylinderEndVertex + vectorClosestPointCylinder;
            }
            distanceVector = sphereCenter - closestPointCylinder;
            if (distanceVector.norm() < radiusSphere) { distanceIntersectionDirection = -1; }
            closestPointSphere = sphereCenter - (distanceVector.normalized() * radiusSphere);
        } else {
            //closest point on cylinder lies  on curved surface area of cylinder
            s = projection / axisSegment.squaredNorm();
            closestPointCylinderAxis = cylinderStartVertex + s * axisSegment;
            distanceVector = (sphereCenter - closestPointCylinderAxis);
            if (distanceVector.norm() < radiusSphere + radiusCylinder) { distanceIntersectionDirection = -1; }
            distanceVector = distanceVector.normalized();
            closestPointCylinder = closestPointCylinderAxis + (distanceVector * radiusCylinder);
            closestPointSphere = sphereCenter - (distanceVector * radiusSphere);

        }
        double distanceMagnitude = (closestPointSphere - closestPointCylinder).norm() * distanceIntersectionDirection;

        return std::make_tuple(distanceMagnitude, closestPointSphere, closestPointCylinder);
    }

/**
 * @brief Calculate the closest distance between a sphere and an oriented box, along with the
 *        respective closest points on both objects.
 *
 * @param radiusSphere   Radius of the sphere
 * @param sphereCenter   center of the sphere in world coordinates
 * @param boxCenter      center of the box in world coordinates
 * @param boxDimensions  Dimensions (width, height, depth) of the box
 * @param boxOrientation Orientation of the box (quaternion)
 * @return std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
 *         - distance between the sphere surface and the box surface (>= 0)
 *         - closest point on the sphere
 *         - closest point on the box
 */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereBox(double const &radiusSphere, Eigen::Vector3d const &sphereCenter,
                               Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                               Eigen::Quaterniond const &boxOrientation) {



        // Step 1: Convert sphere center to box local coordinates
        Eigen::Vector3d sphereCenterShifted = sphereCenter - boxCenter;
        Eigen::Quaterniond boxOrientationInv = boxOrientation.inverse();
        Eigen::Vector3d localSphereCenter = boxOrientationInv * sphereCenterShifted;

        // Half-extents of the box in local coordinates
        Eigen::Vector3d halfExtents = boxDimensions * 0.5;

        // Step 2 & 3: Clamp localSphereCenter to box’s local extents
        Eigen::Vector3d localClosestPoint;
        localClosestPoint.x() = std::max(-halfExtents.x(), std::min(localSphereCenter.x(), halfExtents.x()));
        localClosestPoint.y() = std::max(-halfExtents.y(), std::min(localSphereCenter.y(), halfExtents.y()));
        localClosestPoint.z() = std::max(-halfExtents.z(), std::min(localSphereCenter.z(), halfExtents.z()));

        // Step 4: Transform localClosestPoint back to world coordinates
        Eigen::Vector3d closestPointOnBox = boxCenter + boxOrientation * localClosestPoint;

        // Step 5: Compute the distance from the sphere center to the box’s closest point
        Eigen::Vector3d diff = closestPointOnBox - sphereCenter;
        double distBoxToCenter = diff.norm();

        double distanceSphereToBoxSurface = 0.0;
        Eigen::Vector3d closestPointOnSphere = sphereCenter;  // default

        if (distBoxToCenter > radiusSphere) {
            // Spheres does not intersect the box. Distance > 0
            distanceSphereToBoxSurface = distBoxToCenter - radiusSphere;

            // Step 6: Closest point on the sphere’s surface
            Eigen::Vector3d direction = diff.normalized();
            closestPointOnSphere = sphereCenter + direction * radiusSphere;
        } else {
            // They overlap or just touch (distance is 0)
            distanceSphereToBoxSurface = 0.0;
            // In overlapping case, "closestPointOnSphere" could be left as the center, or
            // any point on the intersection. We leave it as sphereCenter by default.
        }

        return std::make_tuple(distanceSphereToBoxSurface, closestPointOnSphere, closestPointOnBox);
    }

/**
 * @brief Calculate distance and closest points between two oriented boxes using FCL.
 *
 * @param boxCenter1       center of the first box in world coordinates
 * @param boxDimensions1   Dimensions (width, height, depth) of the first box
 * @param boxOrientation1  Orientation (quaternion) of the first box
 * @param boxCenter2       center of the second box in world coordinates
 * @param boxDimensions2   Dimensions (width, height, depth) of the second box
 * @param boxOrientation2  Orientation (quaternion) of the second box
 * @return std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
 *         - The minimum distance between the two boxes (can be negative if boxes intersect).
 *         - The closest point on the first box.
 *         - The closest point on the second box.
 */

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxBox(Eigen::Vector3d const &boxCenter1, Eigen::Vector3d const &boxDimensions1,
                            Eigen::Quaterniond const &boxOrientation1, Eigen::Vector3d const &boxCenter2,
                            Eigen::Vector3d const &boxDimensions2,
                            Eigen::Quaterniond const &boxOrientation2) {
        // 1. Create FCL box geometry for each cuboid
        //    FCL’s Box constructor expects half-extents, but as of v0.6.0 it actually wants full extents.
        //    So pass the actual dimensions (width, height, depth) directly.
        fcl::Boxd fclBox1(boxDimensions1.x(), boxDimensions1.y(), boxDimensions1.z());
        fcl::Boxd fclBox2(boxDimensions2.x(), boxDimensions2.y(), boxDimensions2.z());

        // 2. Construct transforms (world transform of each box)
        //    FCL uses fcl::Transform3<double>, which is essentially Eigen::Isometry3d.
        fcl::Transform3d tf1, tf2;

        // Set rotation from boxOrientation (quaternion) to rotation matrix
        tf1.linear() = boxOrientation1.toRotationMatrix();
        // Set translation
        tf1.translation() = boxCenter1;

        tf2.linear() = boxOrientation2.toRotationMatrix();
        tf2.translation() = boxCenter2;

        // 3. Create FCL collision objects for each box
        auto co1 = std::make_shared<fcl::CollisionObjectd>(
                std::shared_ptr<fcl::CollisionGeometryd>(&fclBox1, [](fcl::CollisionGeometryd *) {}), tf1);
        auto co2 = std::make_shared<fcl::CollisionObjectd>(
                std::shared_ptr<fcl::CollisionGeometryd>(&fclBox2, [](fcl::CollisionGeometryd *) {}), tf2);

        // 4. Set up distance request
        fcl::DistanceRequestd request;
        request.enable_nearest_points = true;   // We want the closest points
        request.enable_signed_distance = true;  // So we can get negative distance if boxes intersect
        request.gjk_solver_type = fcl::GJKSolverType::GST_LIBCCD;
        request.rel_err = 0.0;
        request.abs_err = 0.0;
        request.distance_tolerance = 1e-6;

        // 5. Distance result
        fcl::DistanceResultd result;

        // 6. Perform the distance query
        double dist = fcl::distance(co1.get(), co2.get(), request, result);

        // FCL returns:
        //   - result.min_distance: the minimum distance (negative if boxes intersect)
        //   - result.nearest_points[0]: the closest point on object1
        //   - result.nearest_points[1]: the closest point on object2

        // 7. Convert FCL’s result to Eigen
        Eigen::Vector3d closestPointBox1 = result.nearest_points[0];
        Eigen::Vector3d closestPointBox2 = result.nearest_points[1];

        return std::make_tuple(dist, closestPointBox1, closestPointBox2);
    }


/**
 * @brief Calculate the signed distance and closest points between an oriented box and a cylinder.
 *
 * @param boxCenter       center of the box (world coords)
 * @param boxDimensions   Dimensions of the box (width, height, depth)
 * @param boxOrientation  Orientation (quaternion) of the box
 * @param radiusCylinder  Radius of the cylinder
 * @param cylinderHeight  Total height (length) of the cylinder
 * @param cylinderCenter  center of the cylinder (world coords). This is the midpoint of the cylinder.
 * @param cylinderAxis    A normalized 3D vector that specifies the cylinder’s axis direction in world coords
 *
 * @return std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
 *    - distance: signed distance between box and cylinder
 *        (> 0 => separated, = 0 => touching, < 0 => intersection)
 *    - closest point on the box (in world coords)
 *    - closest point on the cylinder (in world coords)
 */

    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceBoxCylinder(Eigen::Vector3d const &boxCenter, Eigen::Vector3d const &boxDimensions,
                                 Eigen::Quaterniond const &boxOrientation, double const &radiusCylinder,
                                 double const &cylinderHeight,
                                 Eigen::Vector3d const &cylinderCenter, Eigen::Vector3d const &cylinderAxis) {
        // -------------------------------------------------------------
        // 1. Construct the box geometry
        //    FCL’s Box uses full extents (width, height, depth).
        // -------------------------------------------------------------
        fcl::Boxd fclBox(boxDimensions.x(), boxDimensions.y(), boxDimensions.z());

        // -------------------------------------------------------------
        // 2. Construct the cylinder geometry
        //    FCL’s Cylinder(r, lz) => r = radius, lz = length
        //    So we pass (radiusCylinder, cylinderHeight).
        // -------------------------------------------------------------
        fcl::Cylinderd fclCylinder(radiusCylinder, cylinderHeight);

        // -------------------------------------------------------------
        // 3. Build the transform for the box
        //    (position + rotation from boxOrientation)
        // -------------------------------------------------------------
        fcl::Transform3d tfBox;
        tfBox.linear() = boxOrientation.toRotationMatrix();
        tfBox.translation() = boxCenter;

        // -------------------------------------------------------------
        // 4. Build the transform for the cylinder
        //    - We want the cylinder’s local Z-axis to align with cylinderAxis
        //    - The cylinder is centered at cylinderCenter
        // -------------------------------------------------------------
        // Ensure cylinderAxis is normalized. If not, we normalize here:
        Eigen::Vector3d axisNorm = cylinderAxis.normalized();

        // Create rotation that aligns local Z with axisNorm
        Eigen::Quaterniond qCyl = Eigen::Quaterniond::FromTwoVectors(
                Eigen::Vector3d::UnitZ(), axisNorm
        ).normalized();

        fcl::Transform3d tfCyl;
        tfCyl.linear() = qCyl.toRotationMatrix();
        tfCyl.translation() = cylinderCenter;

        // -------------------------------------------------------------
        // 5. Create collision objects for each shape
        //    We wrap the geometry plus transform in a CollisionObject
        // -------------------------------------------------------------
        auto coBox = std::make_shared<fcl::CollisionObjectd>(
                std::shared_ptr<fcl::CollisionGeometryd>(&fclBox, [](fcl::CollisionGeometryd *) {}),
                tfBox
        );

        auto coCyl = std::make_shared<fcl::CollisionObjectd>(
                std::shared_ptr<fcl::CollisionGeometryd>(&fclCylinder, [](fcl::CollisionGeometryd *) {}),
                tfCyl
        );

        // -------------------------------------------------------------
        // 6. Distance request: We want nearest points + signed distance
        // -------------------------------------------------------------
        fcl::DistanceRequestd request;
        request.enable_nearest_points = true;   // get the actual nearest points
        request.enable_signed_distance = true;  // so we can detect penetration with negative distance
        request.gjk_solver_type = fcl::GJKSolverType::GST_LIBCCD;
        fcl::DistanceResultd result;

        // -------------------------------------------------------------
        // 7. Perform the distance query
        //    fcl::distance(...) returns the same value as result.min_distance
        // -------------------------------------------------------------
        double dist = fcl::distance(coBox.get(), coCyl.get(), request, result);

        // -------------------------------------------------------------
        // 8. Extract the closest points
        //    They are in world coordinates
        // -------------------------------------------------------------
        Eigen::Vector3d ptOnBox = result.nearest_points[0];
        Eigen::Vector3d ptOnCylinder = result.nearest_points[1];

        // Return the distance + closest points
        return std::make_tuple(dist, ptOnBox, ptOnCylinder);
    }

/**
 * @brief Calculate the signed distance between two oriented cylinders,
 *        as well as their respective closest points in world coordinates.
 *
 * FCL’s Cylinder(r, lz) is aligned with the local Z-axis, from -lz to +lz.
 * The user provides radius, total height, center, and axis for each cylinder.
 *
 * @param radiusCylinder1       Radius of the first cylinder
 * @param cylinderHeight1       Total height of the first cylinder
 * @param cylinderCenter1       World-space center of the first cylinder (midpoint along its axis)
 * @param cylinderAxis1         Normalized direction (world-space) for the first cylinder’s axis
 * @param radiusCylinder2       Radius of the second cylinder
 * @param cylinderHeight2       Total height of the second cylinder
 * @param cylinderCenter2       World-space center of the second cylinder
 * @param cylinderAxis2         Normalized direction for the second cylinder’s axis
 *
 * @return std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
 *    - distance: signed distance between the cylinders
 *       ( > 0 => separated by that amount,
 *         = 0 => touching,
 *         < 0 => intersecting by |distance| )
 *    - closest point on Cylinder1 (world coords)
 *    - closest point on Cylinder2 (world coords)
 */
    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceCylinderCylinder(double radiusCylinder1, double cylinderHeight1,
                                      const Eigen::Vector3d &cylinderCenter1,
                                      const Eigen::Vector3d &cylinderAxis1,
                                      double radiusCylinder2, double cylinderHeight2,
                                      const Eigen::Vector3d &cylinderCenter2,
                                      const Eigen::Vector3d &cylinderAxis2) {
        // -----------------------------
        // 1. Create FCL cylinder shapes
        //    FCL’s Cylinder(r, lz) uses 'lz' as half-length
        // -----------------------------

        std::shared_ptr<fcl::CollisionGeometryd> cylGeom1 = std::make_shared<fcl::Cylinderd>(radiusCylinder1,
                                                                                             cylinderHeight1);
        std::shared_ptr<fcl::CollisionGeometryd> cylGeom2 = std::make_shared<fcl::Cylinderd>(radiusCylinder2,
                                                                                             cylinderHeight2);

        // -----------------------------
        // 2. Compute transforms (world) for each cylinder
        //    - Must rotate local Z-axis => user’s cylinderAxis
        //    - Then translate to cylinderCenter
        // -----------------------------
        // Normalize axes if not already
        Eigen::Vector3d axis1 = cylinderAxis1.normalized();
        Eigen::Vector3d axis2 = cylinderAxis2.normalized();

        // Cylinder1 rotation
        Eigen::Quaterniond q1 = Eigen::Quaterniond::FromTwoVectors(
                Eigen::Vector3d::UnitZ(), axis1
        ).normalized();

        // Cylinder2 rotation
        Eigen::Quaterniond q2 = Eigen::Quaterniond::FromTwoVectors(
                Eigen::Vector3d::UnitZ(), axis2
        ).normalized();

        // Build FCL transforms
        fcl::Transform3d tf1, tf2;
        tf1.linear() = q1.toRotationMatrix();
        tf1.translation() = cylinderCenter1;

        tf2.linear() = q2.toRotationMatrix();
        tf2.translation() = cylinderCenter2;

        // -----------------------------
        // 3. Create CollisionObjects
        //    - pair geometry + transform
        // -----------------------------
        // NOTE: The somewhat unusual shared_ptr usage is
        //       to satisfy FCL’s internal reference counting.

        fcl::CollisionObjectd cylinder1(cylGeom1, tf1);
        fcl::CollisionObjectd cylinder2(cylGeom2, tf2);

        // -----------------------------
        // 4. Set up distance request (enable nearest points + signed distance)
        // -----------------------------
        fcl::DistanceRequestd request;
        request.enable_nearest_points = true;   // get the actual closest points
        request.enable_signed_distance = true;  // negative if they intersect
        request.gjk_solver_type = fcl::GJKSolverType::GST_LIBCCD;

        // Increase tolerances
        request.distance_tolerance = 1e-5;   // or even 1e-4
        request.rel_err           = 1e-6;
        request.abs_err           = 1e-6;
        // Storage for results
        fcl::DistanceResultd result;

        // -----------------------------
        // 5. Run the distance query
        // -----------------------------
        double dist = fcl::distance(&cylinder1, &cylinder2, request, result);

        // dist == result.min_distance
        // dist < 0 => cylinders intersect
        // dist = 0 => contact
        // dist > 0 => separation

        // -----------------------------
        // 6. Extract closest points in world coords
        // -----------------------------
        Eigen::Vector3d ptOnCyl1 = result.nearest_points[0];
        Eigen::Vector3d ptOnCyl2 = result.nearest_points[1];

        return std::make_tuple(dist, ptOnCyl1, ptOnCyl2);
    }


    std::tuple<double, Eigen::Vector3d, Eigen::Vector3d, Eigen::Vector3d>
    calculateDistanceSphereLineSweptSphere(double const &sphereRadius, Eigen::Vector3d const &sphereCenter,
                                           Eigen::Vector3d const &linkSegmentV0,
                                           Eigen::Vector3d const &linkSegmentV1,
                                           double const &linkRadius) {


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
        Vector3d closestPoint = linkSegmentV0 + (s * axisSegment);

        // Vector direction from sphere center to the closest point
        Vector3d distanceVector = closestPoint - sphereCenter;
        Vector3d distanceNormalized = distanceVector.normalized();
        double distanceMagnitude = distanceVector.norm() - sphereRadius - linkRadius;

        distanceVector = distanceMagnitude * distanceNormalized;

        Vector3d closestPointSphere = sphereCenter + (sphereRadius * distanceNormalized);
        Vector3d closestPointLinkSegment = closestPoint - (linkRadius * distanceNormalized);
        return std::make_tuple(distanceMagnitude, distanceVector, closestPointSphere, closestPointLinkSegment);

    }
}