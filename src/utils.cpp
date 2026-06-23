
//
// Created by shobhit on 12.10.24.
//

#include <TUM_SJ_ObstacleAvoidanceLib/utils.h>
#include <Eigen/Geometry>

using namespace AndreiUtils;

using namespace Eigen;
using namespace ObstacleAvoidance;
using namespace std;


Eigen::Matrix4d ObstacleAvoidance::trvec2tform(const Eigen::Vector3d &translation) {
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity(); // Start with an identity matrix
    transform.block<3, 1>(0, 3) = translation; // Insert the translation vector
    return transform;
}

Eigen::Matrix4d ObstacleAvoidance::eul2tform(const Eigen::Vector3d &eulerAngles) {
    // Extract individual angles
    double roll = eulerAngles(0); // Roll (rotation about X-axis)
    double pitch = eulerAngles(1); // Pitch (rotation about Y-axis)
    double yaw = eulerAngles(2); // Yaw (rotation about Z-axis)

    // Calculate rotation matrix components
    Eigen::Matrix3d rotation;
    rotation = Eigen::AngleAxisd(yaw, Eigen::Vector3d::UnitZ()) *
               Eigen::AngleAxisd(pitch, Eigen::Vector3d::UnitY()) *
               Eigen::AngleAxisd(roll, Eigen::Vector3d::UnitX());

    // Create transformation matrix
    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    transform.block<3, 3>(0, 0) = rotation;

    return transform;
}

Eigen::Matrix4d ObstacleAvoidance::convertEulerToTransform(const Eigen::Vector3d &angles, const std::string &seq) {
    Eigen::Matrix3d Rz = Eigen::AngleAxisd(angles(2), Eigen::Vector3d::UnitZ()).toRotationMatrix();
    Eigen::Matrix3d Ry = Eigen::AngleAxisd(angles(1), Eigen::Vector3d::UnitY()).toRotationMatrix();
    Eigen::Matrix3d Rx = Eigen::AngleAxisd(angles(0), Eigen::Vector3d::UnitX()).toRotationMatrix();

    // Assuming ZYX order
    Eigen::Matrix3d rotation = Rz * Ry * Rx;

    Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
    transform.block<3, 3>(0, 0) = rotation;
    return transform;
}


Eigen::Matrix3d ObstacleAvoidance::skewSymmetric(Eigen::Vector3d const &v) {
    return (Eigen::Matrix3d() << 0, -v.z(), v.y(),
            v.z(), 0, -v.x(),
            -v.y(), v.x(), 0).finished();
}

Eigen::MatrixXd ObstacleAvoidance::computeAdjoint(Eigen::Matrix4d const &T) {
    Eigen::Matrix3d R = T.block<3, 3>(0, 0);
    Eigen::Vector3d p = T.block<3, 1>(0, 3);
    Eigen::MatrixXd Ad(6, 6);
    Ad << R.transpose(), -R.transpose() * skewSymmetric(p),
            Eigen::Matrix3d::Zero(), R.transpose();
    return Ad;
}

MatrixXd ObstacleAvoidance::vectorMatrixToEigenMatrix(std::vector<std::vector<double> > const &vec) {
    MatrixXd mat(vec.size(), vec[0].size());
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = 0; j < vec[i].size(); ++j) {
            mat(i, j) = vec[i][j];
        }
    }
    return mat;
}

Matrix<double, 1, Eigen::Dynamic> ObstacleAvoidance::vectorToEigenMatrixRow(std::vector<double> const &vec) {
    Matrix<double, 1, Eigen::Dynamic> rowMatrix(1, vec.size());
    rowMatrix << Eigen::Map<const Matrix<double, 1, Eigen::Dynamic>>(vec.data(), 1, vec.size());
    return rowMatrix;
}

Eigen::VectorXd ObstacleAvoidance::stdVectorToEigenVector(const vector<double> &vec) {
    VectorXd column(vec.size());
    column << Eigen::Map<const VectorXd>(vec.data(), vec.size(), 1);
    return column;
}


Eigen::VectorXd ObstacleAvoidance::generateSequence(double startValue, double ts, double lastValue) {
    int numSamples = static_cast<int>((lastValue - startValue) / ts) + 1;
    Eigen::VectorXd sequence(numSamples);
    for (int i = 0; i < numSamples; ++i) {
        sequence(i) = startValue + i * ts;
    }
    return sequence;
}


Eigen::Vector3d ObstacleAvoidance::computeOrientationError(Eigen::Matrix4d const &T_current, Eigen::VectorXd const &q) {
    if (q.size() != 4) {
        throw std::runtime_error("Vector size must be exactly 4 to form a quaternion.");
    }
    // Eigen::Quaterniond expects (w, x, y, z)
    auto desiredQuaternion = Eigen::Quaterniond(q[0], q[1], q[2], q[3]);

    Eigen::Matrix3d rotation = T_current.block<3, 3>(0, 0);
    Eigen::Quaterniond currentQuaternion(rotation);
    Eigen::Quaterniond quaternionError = currentQuaternion.conjugate() * desiredQuaternion;
    Eigen::AngleAxisd angleAxis(quaternionError);
    // Angle-axis interpreted as an angular velocity over a unit time step.
    Eigen::Vector3d angularVelocityCorrection = angleAxis.angle() * angleAxis.axis();
    // Express the correction in the base frame.
    Eigen::Vector3d angularVelocityCorrectionBase = rotation * angularVelocityCorrection;
    return angularVelocityCorrectionBase;
}

std::vector<double> ObstacleAvoidance::EigenVectorToStdVector(VectorXd const &eigen_vector) {
    std::vector<double> stdVector(eigen_vector.data(), eigen_vector.data() + eigen_vector.size());
    return stdVector;
}


void ObstacleAvoidance::writeMatrixToCSV(std::string const &filename, Eigen::MatrixXd const &matrix) {
    std::ofstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < matrix.rows(); ++i) {
            for (int j = 0; j < matrix.cols(); ++j) {
                file << matrix(i, j);
                if (j != matrix.cols() - 1)
                    file << ", ";
            }
            file << "\n";
        }
        file.close();
    } else {
        std::cerr << "Could not open the file for writing.\n";
    }
}

Eigen::MatrixXd ObstacleAvoidance::readMatrixFromCSV(const std::string &filename) {
    std::vector<double> matrixEntries;

    // Initialize variables needed for the loop
    std::ifstream file(filename);
    std::string line;
    int numRows = 0;
    int numCols = 0;

    // First pass to fill in the entries
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string item;
        while (std::getline(ss, item, ',')) {
            matrixEntries.push_back(stod(item));
        }
        ++numRows;
    }

    // Compute the number of columns
    numCols = matrixEntries.size() / numRows;

    // Now we'll fill the Eigen matrix with the entries
    Eigen::MatrixXd matrix = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> >(
            matrixEntries.data(), numRows, numCols);

    return matrix;
}

Vector4d ObstacleAvoidance::computeQuaternionFromMatrix(Eigen::Matrix4d const &transformationMatrix) {
    Vector4d quaternionOutput;
    Matrix3d const rotationMatrix = transformationMatrix(seq(0, 2), seq(0, 2));
    Quaterniond quaternion(rotationMatrix);
    auto coefficients = quaternion.coeffs();
    quaternionOutput(0) = coefficients[3];
    quaternionOutput(seq(1, 3)) = coefficients(seq(0, 2));
    return quaternionOutput;
}
