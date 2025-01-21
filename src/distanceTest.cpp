//
// Created by shobhit on 26.12.24.
//


#include <DistanceObjects.h>
#include <Eigen/Dense>
#include <iostream>
#include<AndreiUtils/utilsString.h>
#include <AndreiUtils/utilsEigenGeometry.hpp>
#include <privateUtils.h>
using namespace Eigen;
using namespace std;
using namespace ObstacleAvoidance;

int main() {

    //Box and Sphere Distance Test
    std::string testString = "Teststring_45";
    auto result = AndreiUtils::splitString(testString,"_");
    std::cout<<testString<<std::endl;
    std::cout<<"Part one of string: "<<result[0]<<std::endl;
    std::cout<<"Part second of string: "<<result[1]<<std::endl;

    MatrixXd testMatrix(0,4);
    auto matrixCoder = EigenToCoder(testMatrix);
    cout<<matrixCoder.size(0)<<endl;
    cout<<matrixCoder.size(1)<<endl;


//    testMatrix(0,4)
   // std::cout<<result[1]<<std::endl;

//    Vector3d
    Vector3d boxCenter(-3, -3, 0);
    Eigen::Quaterniond boxOrientation(1, 0, 0, 0);
    Vector3d boxDimensions(2, 2, 2);

    Vector3d boxCenter2(3, 3, 1);
    Eigen::Quaterniond boxOrientation2(1, 0, 0, 0);
    Vector3d boxDimensions2(2, 2, 2);
    //    Eigen::Quaterniond boxOrientation2(0.9238795325, 0, 0, 0.3826834324);

    Vector3d sphereCenter(1, 4, 0);
    double sphereRadius = 1;

    Vector3d cylinderAxis1(1, 0, 0);
    double cylinderHeight1 = 2;
    Vector3d cylinderCenter1(3, -3, 0);
    double cylinderRadius1 = 1;

    Vector3d cylinderAxis2(1, 1, 0);
    double cylinderHeight2 = 2;
    Vector3d cylinderCenter2(-3, 0, 0);
    double cylinderRadius2 = 1;

    auto [distance, closestPoint1, closestPoint2] = calculateDistanceCylinderCylinder(cylinderRadius1, cylinderHeight1,
                                                                                      cylinderCenter1, cylinderAxis1,
                                                                                      cylinderRadius2,
                                                                                      cylinderHeight2, cylinderCenter2,
                                                                                      cylinderAxis2);
    auto [distanceBoxCylinder, closesetPointBox, closesetPointCylinder] = calculateDistanceBoxCylinder(boxCenter,
                                                                                                       boxDimensions,
                                                                                                       boxOrientation,
                                                                                                       cylinderRadius1,
                                                                                                       cylinderHeight1,
                                                                                                       cylinderCenter1,
                                                                                                       cylinderAxis1);

    auto [distanceSphereCylinder, closesetPointSphere, closesetPointCylinder1] = calculateDistanceSphereCylinder(
            sphereRadius, sphereCenter, cylinderRadius1, cylinderHeight1, cylinderCenter1, cylinderAxis1);

    auto [distanceSphereBox, closesetPointSphere1, closesetPointBox1] = calculateDistanceSphereBox(sphereRadius,
                                                                                                   sphereCenter,
                                                                                                   boxCenter,
                                                                                                   boxDimensions,
                                                                                                   boxOrientation);

    auto [distanceBoxBox, closestPointBox1, closestPointBox2] = calculateDistanceBoxBox(boxCenter, boxDimensions,
                                                                                        boxOrientation, boxCenter2,
                                                                                        boxDimensions2,
                                                                                        boxOrientation2);

    cout << "distance: " << distance << endl;
    cout << "CLosest Point on Cylinder 1: \n" << closestPoint1 << endl;
    cout << "CLosest Point on Cylinder 2: \n" << closestPoint2 << endl;

    cout << "distance between Box and Cylinder: " << distanceBoxCylinder << endl;
    cout << "CLosest Point on Box: \n" << closesetPointBox << endl;
    cout << "CLosest Point on Cylinder 1: \n" << closesetPointCylinder << endl;

    cout << "distance between Sphere and Cylinder: " << distanceSphereCylinder << endl;
    cout << "CLosest Point on Sphere: \n" << closesetPointSphere << endl;
    cout << "CLosest Point on Cylinder 1: \n" << closesetPointCylinder1 << endl;

    cout << "distance between Sphere and Box: " << distanceSphereBox << endl;
    cout << "CLosest Point on Sphere: \n" << closesetPointSphere1 << endl;
    cout << "CLosest Point on Box: \n" << closesetPointBox1 << endl;

    cout << "distance between Box1 and Box2: " << distanceBoxBox << endl;
    cout << "CLosest Point on Box1 : \n" << closestPointBox1 << endl;
    cout << "CLosest Point on Box2: \n" << closestPointBox2 << endl;


//    cout<<cylinderAxis1.normalized()<<endl;

    /* Eigen::Quaterniond q1 = Eigen::Quaterniond::FromTwoVectors(
     Eigen::Vector3d::UnitZ(),cylinderAxis2.normalized()
     ).normalized();


     cout<<cylinderAxis2.normalized()<<endl;
     cout<<(q1.toRotationMatrix()*Eigen::Vector3d::UnitZ()).normalized()<<endl;*/

//    cout<<boxOrientation2.toRotationMatrix()<<endl;
/*    VectorXd  aSegment;
    cout<<"SIze of aSegment: "<<aSegment.size()<<" \n"<<aSegment<<endl;*/
    return 0;

}