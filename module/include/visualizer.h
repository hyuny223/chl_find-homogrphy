#pragma once

#include <iostream>
#include <vector>

#include "eigen-3.4.0/Eigen/Dense"


void visualizer(std::vector<std::vector<double>> values, const Eigen::MatrixXd gt, Eigen::MatrixXd pred);
