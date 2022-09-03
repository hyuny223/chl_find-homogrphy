#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <tuple>

#include "RANSAC.h"

Linear::Linear(const std::vector<std::vector<double>>& datas, const int& dataNum)
: mDatas(datas), mDataNum(dataNum)
{}

void Linear::run(std::mt19937& gen, const double& margin)
{
    std::uniform_int_distribution<int> dis(0, mDataNum-1);
    int row1, row2;
    double x1, y1, x2, y2;

    while(1)
    {
        row1 = dis(gen), row2 = dis(gen);
        x1 = mDatas[row1][0], y1 = mDatas[row1][1];
        x2 = mDatas[row2][0], y2 = mDatas[row2][1];
        if(x1 != x2) // 기울기의 분모가 0이 되는 것을 막기 위하여 제외
        {
            break;
        }
    }

    auto [gradient, intercept] = computeLinearParameter(x1,y1,x2,y2); // 기울기, y절편

    double sumRes{0};
    std::vector<int> inliers;
    inliers.reserve(mDataNum);

    for(int row = 0; row < mDataNum; ++row)
    {
        double x = mDatas[row][0], y = mDatas[row][1];
        double res = std::abs(gradient * x - y + intercept) / std::sqrt(gradient*gradient + 1); // 점과 직선과의 거리

        if(res < margin)
        {
            res = 0;
            inliers.emplace_back(row);
        }

        sumRes += res;
    }
    mGradient = gradient;
    mIntercept = intercept;
    mResidual = sumRes;
    mInliers = inliers;
}

std::tuple<double,double> Linear::computeLinearParameter(const double& x1, const double& y1, const double& x2, const double& y2)
{
    double dx = x1 - x2;
    double dy = y1 - y2;
    double gradient = dy / dx;

    double intercept = y1 - gradient * x1;

    return std::tuple<double, double>(gradient,intercept);
}

double Linear::getResidual()
{
    return mResidual;
}

std::tuple<double, double> Linear::getParameter()
{
    return std::tuple<double, double>(mGradient, mIntercept);
}

std::vector<int> Linear::getInliers()
{
    return mInliers;
}
