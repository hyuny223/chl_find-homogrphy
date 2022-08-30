#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <random>

template <typename MODEL>
class RANSAC
{
    public:
        RANSAC(MODEL& model, const int& iteration, const double& margin, const double& threshold)
        : mModel(model), mIteration(iteration), mMargin(margin), mThreshold(threshold)
        {}

        MODEL run()
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            for(int iter=0; iter < mIteration; ++iter)
            {
                if(this->iterate(gen))
                {
                    if(mBestModelRes < mThreshold)
                    {

                        return mModel;
                    }
                }
            }

            return mModel;
        }

    protected:
        bool iterate(std::mt19937& gen)
        {
            mModel.run(gen, mMargin);
            auto res = mModel.getResidual();

            std::cout.precision(10);
            std::cout << res << std::endl;

            if(res < mBestModelRes)
            {
                mBestModelRes = res;
                return true;
            }
            return false;
        }

        int mDataNum, mIteration;
        double mMargin, mThreshold;
        double mBestModelRes=900000, mBestGradient, mBestIntercept;

        MODEL mModel;
};


class Linear
{
    private:
        double mGradient, mIntercept, mResidual;
        int mDataNum;
        std::vector<std::vector<double>> mDatas;

    public:
        Linear(const std::vector<std::vector<double>>& datas, const int& dataNum);
        void run(std::mt19937& gen, const double& margin);
        std::tuple<double,double> computeLinearParameter(const double& x1, const double& y1, const double& x2, const double& y2);
        std::tuple<double, double> getParameter();
        double getResidual();
};
