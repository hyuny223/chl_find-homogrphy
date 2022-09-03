#include <iostream>

#include "eigen-3.4.0/Eigen/Dense"
#include "opencv2/opencv.hpp"

#include "csv_reader.h"
#include "compute.h"
#include "RANSAC.h"
#include "visualizer.h"

int main(int argc, char** argv)
{
    std::string data_path(argv[1]);
    std::string out_path(argv[2]);

    std::vector<std::vector<double>> values(30,std::vector<double>(5));
    std::vector<std::vector<double>> outs(3,std::vector<double>(4));
    readCsv(data_path, values, 5);
    readCsv(out_path, outs, 4);

    Eigen::MatrixXd out(3,4);

    for(int row = 0; row < 3; ++row)
    {
        double h0 = outs[row][0], h1 = outs[row][1], h2 = outs[row][2], h3 = outs[row][3];

        Eigen::MatrixXd r(1,4);
        r << h0, h1, h2, h3;

        out.block<1,4>(row, 0) = r;
    }

    std::size_t len = values.size();

    Linear model(values, len);
    RANSAC<Linear> ransac(model, 300, 200, 4000);
    model = ransac.run();
    auto inliers_idx = model.getInliers();
    std::vector<std::vector<double>> inliers;
    inliers.reserve(len);
    for(const int& i:inliers_idx)
    {
        inliers.emplace_back(values.at(i));
    }

    Eigen::MatrixXd C(3,4), point3D(inliers.size(),4);
    computeSVD(inliers, inliers.size(), C, point3D);

    Eigen::MatrixXd pred = compute2D(C, point3D);
    Eigen::MatrixXd gt = compute2D(out, point3D);

    computeRMSE(inliers, gt, pred);
    visualizer(inliers, gt, pred);

}
