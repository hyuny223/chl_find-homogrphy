#include <iostream>
#include <vector>
#include <cmath>

#include "opencv2/opencv.hpp"
#include "eigen-3.4.0/Eigen/Dense"

void visualizer(std::vector<std::vector<double>> values, const Eigen::MatrixXd gt, Eigen::MatrixXd pred)
{
    cv::Mat board = cv::Mat::zeros(cv::Size(2000,1000), CV_8UC3);

    std::size_t len = values.size();

    for(int i = 0; i < len; ++i)
    {
        auto val_x = static_cast<int>(values[i][0]), val_y = static_cast<int>(values[i][1]);
        auto gt_x = static_cast<int>(std::round(gt(i,0))), gt_y = static_cast<int>(std::round(gt(i,1)));
        auto pred_x = static_cast<int>(std::round(pred(i,0))), pred_y = static_cast<int>(std::round(pred(i,1)));

        cv::circle(board, cv::Point(val_x, val_y), 6, cv::Scalar(255, 255, 255));
        cv::circle(board, cv::Point(gt_x, gt_y), 6, cv::Scalar(0, 0, 255));
        cv::circle(board, cv::Point(pred_x, pred_y), 6, cv::Scalar(0, 255, 0));
    }
    cv::imshow("result",board);
    while(1)
    {
        if(cv::waitKey(0) == 27)
        {
            break;
        }
    }
}
