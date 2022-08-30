#pragma once

#include <iostream>
#include <vector>

#include "eigen-3.4.0/Eigen/Dense"

void computeB(const std::vector<std::vector<double>>& values, Eigen::MatrixXd& B, Eigen::MatrixXd& point3D);
void computeSVD(const std::vector<std::vector<double>>& values, const int& dataNum, Eigen::MatrixXd& C, Eigen::MatrixXd& point3D);
void computeC(const Eigen::JacobiSVD<Eigen::MatrixXd>& svd, Eigen::MatrixXd& C);
Eigen::MatrixXd compute2D(const Eigen::MatrixXd& C, const Eigen::MatrixXd& point3D);
void computeMSE(const std::vector<std::vector<double>> values, const Eigen::MatrixXd& gt, const Eigen::MatrixXd& pred);
