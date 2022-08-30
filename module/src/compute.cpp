#include <iostream>
#include <vector>
#include <string>
#include <cmath>

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

void computeRMSE(const std::vector<std::vector<double>> values, const Eigen::MatrixXd& gt, const Eigen::MatrixXd& pred)
{
    std::size_t len = values.size();

    double gtError{0}, predError{0};

    for(int i = 0; i < len; ++i)
    {
        auto val_x = static_cast<int>(values[i][0]), val_y = static_cast<int>(values[i][1]);
        auto gt_x = static_cast<int>(std::round(gt(i,0))), gt_y = static_cast<int>(std::round(gt(i,1)));
        auto pred_x = static_cast<int>(std::round(pred(i,0))), pred_y = static_cast<int>(std::round(pred(i,1)));

        gtError += std::sqrt((val_x - gt_x)*(val_x - gt_x) + (val_y - gt_y)*(val_y - gt_y));
        predError += std::sqrt((val_x - pred_x)*(val_x - pred_x) + (val_y - pred_y)*(val_y - pred_y));
    }

    std::cout << "RMSE of GT H Matrix : " << gtError / len << std::endl;
    std::cout << "RMSE of Predicted H Matrix : " << predError / len << std::endl;
}
