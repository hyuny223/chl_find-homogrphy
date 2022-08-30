#include <iostream>
#include <vector>
#include <string>

#include "eigen-3.4.0/Eigen/Dense"
#include "eigen-3.4.0/Eigen/SVD"

#include "compute.h"

void computeB(const std::vector<std::vector<double>>& values, Eigen::MatrixXd& B, Eigen::MatrixXd& point3D)
{
    int row = 1;
    for(const auto& value : values)
    {
        auto x = value[2], y = value[3], z = value[4];
        auto u = value[0], v = value[1];

        Eigen::MatrixXd A1(1,12), A2(1,12), P(1,4);
        A1 << x, y, z, 1.0, 0.0, 0.0, 0.0, 0.0, -x*u, -y*u, -z*u, -u;
        A2 << 0.0, 0.0, 0.0, 0.0, x, y, z, 1.0, -x*v, -y*v, -z*v, -v;
        P << x, y, z, 1;

        B.block<1,12>(2*row - 2,0) = A1;
        B.block<1,12>(2*row - 1,0) = A2;
        point3D.block<1,4>(row-1, 0) = P;
        ++row;
    }
}

void computeSVD(const std::vector<std::vector<double>>& values, const int& dataNum, Eigen::MatrixXd& C, Eigen::MatrixXd& point3D)
{
    Eigen::MatrixXd B(dataNum*2,12);
    computeB(values, B, point3D);

    Eigen::JacobiSVD<Eigen::MatrixXd> svd(B,Eigen::ComputeFullU | Eigen::ComputeFullV);
    computeC(svd, C);
}

void computeC(const Eigen::JacobiSVD<Eigen::MatrixXd>& svd, Eigen::MatrixXd& C)
{
    auto tmp = svd.matrixV().col(11);

    C.block<1,4>(0,0) = tmp.block<4,1>(0,0);
    C.block<1,4>(1,0) = tmp.block<4,1>(4,0);
    C.block<1,4>(2,0) = tmp.block<4,1>(8,0);
}

Eigen::MatrixXd compute2D(const Eigen::MatrixXd& C, const Eigen::MatrixXd& point3D)
{
    auto point3DTranspose = point3D.transpose();
    auto point2D = (C * point3DTranspose).transpose();
    auto result = point2D.array().colwise() / point2D.col(2).array();

    return result;

}
