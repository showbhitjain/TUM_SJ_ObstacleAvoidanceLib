//
// Created by shobhit on 26.12.24.
//


#include <TUM_SJ_ObstacleAvoidanceLib/DistanceObjects.h>
#include <Eigen/Dense>
#include <iostream>
#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include<AndreiUtils/utilsString.h>
#include <TUM_SJ_ObstacleAvoidanceLib/ObstacleAvoidanceUtils.h>
#include <AndreiUtils/utilsEigenGeometry.hpp>


using namespace Eigen;
using namespace std;
using namespace ObstacleAvoidance;

int main() {
    std::map<std::string,Obstacle> obstaclesMap;

    //Box and Sphere Distance Test
   auto sequence=  generateSequence(4,0.001,6);

    Vector2d timepoints;
    timepoints<<0,4;
    auto addtime = (2+0.001) * VectorXd::Ones(timepoints.size());
    timepoints = timepoints + addtime;
    cout<<timepoints<<endl;
    cout<<sequence<<endl;
    std::map<int,bool> tryMap;
    tryMap.emplace(5,true);
    cout<<"tryMAP KEY:" << tryMap.begin()->first<<endl;
    cout<<"tryMAP value:" << tryMap.begin()->second<<endl;

    std::string testString = "Teststring_45";
    auto result = AndreiUtils::splitString(testString,"_");
    std::cout<<testString<<std::endl;
    std::cout<<"Part one of string: "<<result[0]<<std::endl;
    std::cout<<"Part second of string: "<<result[1]<<std::endl;
    cout<<24.0+std::numeric_limits<double>::quiet_NaN()<<endl;
    Vector3d ut;
    Vector3d zt(3,5,7);

    cout<<"difference :"<<zt-ut<<endl;
    cout<<"Smoothening test: "<<ObstacleAvoidance::smoothingConstraintScheme(0.035,0.01,0.04,2)<<endl;

    Vector3d obstacleCenter = {0.113538,0.0291476,0.629339};
    Vector3d obstacleAxis = {0.908948,0.339189,-0.242415};
    double  obstacleRadius = 0.1;
    double  obstacleHeight = 0.0825;

    Vector3d  linkSphereCenter(0.472804,0.167345,0.431461);
    Vector3d  linkStartingPoint(0.472804,0.167345,0.431461);
    Vector3d  linkEndPoint(0.5545150,0.2000130,0.4314560);
    Vector3d LinkAxis = linkEndPoint - linkStartingPoint;
    double linkNorm = LinkAxis.norm();
    Vector3d linkAxisNormalized = LinkAxis.normalized();
    Vector3d linkCenter = linkStartingPoint + (linkNorm/2)*linkAxisNormalized;

    auto[distanceDebug,closestPointObstacleLink,closestPointOnRobotLink] = calculateDistanceCylinderCylinder(0.1,obstacleHeight,obstacleCenter,obstacleAxis,0.05,linkNorm,linkCenter,linkAxisNormalized);
    cout<<distanceDebug<<endl;
    double linkSphereRadius = 0.05;
   auto[distanceCalc,closestPointSphere,closestPointCylinder]= calculateDistanceSphereCylinder(linkSphereRadius,linkSphereCenter,obstacleRadius,obstacleHeight,obstacleCenter,obstacleAxis);
cout<<"distance Calc: "<<distanceCalc<<endl;
    Vector3d testSphereCenter(0.5545,0.20,0.5211);
    double testSphereRadius = 0.08;
    Vector3d v1 = {0.5545,   -0.0000,    0.7315};

    Vector3d v0 = {0.4665,   -0.0000,    0.7315};
    Vector3d testCylinderCenter = v0 + (0.5 *(v1-v0)) ;
    double testCylinderRadius = 0.1;
    Vector3d testCylinderAxis = (v1 -v0).normalized();
    double testCylinderHeight = (v1-v0).norm();

   /*auto [testDistance,testClosestPointSphere,testClosestPointCylinder]=  calculateDistanceSphereCylinder(testSphereRadius,testSphereCenter,testCylinderRadius,testCylinderHeight,testCylinderCenter,testCylinderAxis);
    cout<<"testDistance: \n"<<testDistance<<endl;
    cout<<"testClosestPointSphere: \n"<<testClosestPointSphere<<endl;
    cout<<"testClosestPointCylinder: \n"<<testClosestPointCylinder<<endl;
*/
    auto [testDistance, testDistanceVector,testClosestPointSphere,testClosestPointCylinder] =  calculateDistanceSphereLineSweptSphere(testSphereRadius,testSphereCenter,v0,v1,testCylinderRadius);
    cout<<"testDistance: \n"<<testDistance<<endl;
    cout<<"testClosestPointSphere: \n"<<testClosestPointSphere<<endl;
    cout<<"testClosestPointCylinder: \n"<<testClosestPointCylinder<<endl;

    // testMatrix(0,4)
// std::cout<<result[1]<<std::endl;
// Vector3d
    Vector3d boxCenter(-3, -3, 0);

    Eigen::Quaterniond boxOrientation(1, 0, 0, 0);
    Vector3d boxDimensions(2, 2, 2);

    Vector3d boxCenter2(3, 3, 1);
    Eigen::Quaterniond boxOrientation2(1, 0, 0, 0);
    Vector3d boxDimensions2(2, 2, 2);
    //    Eigen::Quaterniond boxOrientation2(0.9238795325, 0, 0, 0.3826834324);

    Vector3d sphereCenter(6, 3, 0);
    double sphereRadius = 1;

    Vector3d cylinderAxis1(1,0 , 0);
    double cylinderHeight1 = 4;
    Vector3d cylinderCenter1(2, 1, 0);
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