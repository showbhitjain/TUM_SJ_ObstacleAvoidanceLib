//
// Created by shobhit on 12.10.24.
//

#ifndef TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
#define TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H

#include <AndreiUtils/classes/DualQuaternion.hpp>
#include <Eigen/Dense>
#include <vector>
#include <iostream>
#include <fstream>


namespace ObstacleAvoidance {

    /**
     * @brief Build a homogeneous transformation that only applies a translation.
     * @param translation Translation vector (x, y, z).
     * @return 4x4 homogeneous transformation with identity rotation and the given translation.
     */
    Eigen::Matrix4d trvec2tform(Eigen::Vector3d const &translation);

    /**
     * @brief Build a homogeneous transformation from roll-pitch-yaw Euler angles.
     * @param eulerAngles Euler angles (roll about X, pitch about Y, yaw about Z) in radians.
     * @return 4x4 homogeneous transformation with the corresponding rotation and zero translation.
     */
    Eigen::Matrix4d eul2tform(Eigen::Vector3d const &eulerAngles);

    /**
     * @brief Convert Euler angles to a homogeneous transformation using the given rotation sequence.
     * @param angles Euler angles in radians, ordered to match @p seq.
     * @param seq Rotation sequence (defaults to "ZYX"); the implementation composes Rz * Ry * Rx.
     * @return 4x4 homogeneous transformation with the corresponding rotation and zero translation.
     */
    Eigen::Matrix4d convertEulerToTransform(Eigen::Vector3d const &angles, std::string const &seq = "ZYX");

    /**
     * @brief Build the 3x3 skew-symmetric matrix of a vector.
     * @param v Input 3D vector.
     * @return Skew-symmetric matrix such that skewSymmetric(v) * w == v.cross(w).
     */
    Eigen::Matrix3d skewSymmetric(Eigen::Vector3d const &v);

    /**
     * @brief Compute the 6x6 adjoint (transposed convention) of a homogeneous transformation.
     * @param T 4x4 homogeneous transformation.
     * @return 6x6 adjoint matrix built from the transpose of the rotation and the translation.
     */
    Eigen::MatrixXd computeAdjoint(Eigen::Matrix4d const &T);

    /**
     * @brief Convert a nested std::vector into an Eigen matrix (row-by-row).
     * @param vec Rectangular nested vector; each inner vector is one matrix row.
     * @return Eigen matrix with the same row/column layout as @p vec.
     */
    Eigen::MatrixXd vectorMatrixToEigenMatrix(std::vector<std::vector<double>> const &vec);

    /**
     * @brief Convert a std::vector into a single-row Eigen matrix.
     * @param vec Input values.
     * @return 1xN row matrix containing @p vec.
     */
    Eigen::Matrix<double, 1, Eigen::Dynamic> vectorToEigenMatrixRow(std::vector<double> const &vec);

    /**
     * @brief Convert a std::vector into an Eigen column vector.
     * @param vec Input values.
     * @return Nx1 Eigen vector containing @p vec.
     */
    Eigen::VectorXd stdVectorToEigenVector(std::vector<double> const &vec);

    /**
     * @brief Convert an Eigen vector into a std::vector.
     * @param eigen_vector Input Eigen vector.
     * @return std::vector containing the same values.
     */
    std::vector<double> EigenVectorToStdVector(Eigen::VectorXd const &eigen_vector);

    /**
     * @brief Generate an evenly spaced sequence of values.
     * @param startValue First value of the sequence.
     * @param ts Step size between consecutive values.
     * @param lastValue Upper bound used to determine the number of samples.
     * @return Vector with values startValue, startValue + ts, ... up to (and possibly including) lastValue.
     */
    Eigen::VectorXd generateSequence(double startValue, double ts, double lastValue);

    /**
     * @brief Compute the orientation error between a current pose and a desired orientation.
     * @param T_current Current 4x4 homogeneous transformation.
     * @param desired_q Desired orientation as a quaternion.
     * @return Angular-velocity-like orientation error expressed in the base frame.
     */
    Eigen::Vector3d computeOrientationError(const Eigen::Matrix4d &T_current, const Eigen::Quaterniond &desired_q);

    /**
     * @brief Compute the orientation error between a current pose and a desired orientation.
     * @param T_current Current 4x4 homogeneous transformation.
     * @param q Desired orientation as a 4-element quaternion vector ordered (w, x, y, z).
     * @return Angular-velocity-like orientation error expressed in the base frame.
     */
    Eigen::Vector3d computeOrientationError(const Eigen::Matrix4d &T_current, const Eigen::VectorXd &q);

    /**
     * @brief Write an Eigen matrix to a CSV file (comma separated, one matrix row per line).
     * @param filename Output file path.
     * @param matrix Matrix to write.
     */
    void writeMatrixToCSV(std::string const &filename, Eigen::MatrixXd const &matrix);

    /**
     * @brief Read a matrix from a comma-separated CSV file.
     * @param filename Input file path.
     * @return Matrix reconstructed from the file contents.
     */
    Eigen::MatrixXd readMatrixFromCSV(const std::string& filename);

    /**
     * @brief Extract the orientation of a homogeneous transformation as a quaternion vector.
     * @param transformationMatrix 4x4 homogeneous transformation.
     * @return Quaternion ordered (w, x, y, z).
     */
    Eigen::Vector4d computeQuaternionFromMatrix(Eigen::Matrix4d const& transformationMatrix);

}


#endif //TUM_SJ_OBSTACLEAVOIDANCELIB_UTILS_H
