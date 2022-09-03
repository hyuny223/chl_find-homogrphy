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
            MODEL candidate;

            for(int iter=0; iter < mIteration; ++iter)
            {
                if(this->iterate(gen))
                {
                    if(mBestModelRes < mThreshold)
                    {
                        std::cout << "model!" << std::endl;
                        return mModel;
                    }
                    candidate = mModel;
                }
            }
            return candidate;
        }

    protected:
        bool iterate(std::mt19937& gen)
        {
            mModel.run(gen, mMargin);
            auto res = mModel.getResidual();

            if(res < mBestModelRes)
            {
                mBestModelRes = res;
                return true;
            }
            return false;
        }

        int mDataNum, mIteration;
        double mMargin, mThreshold;
        double mBestModelRes=std::numeric_limits<double>::infinity(), mBestGradient, mBestIntercept;

        MODEL mModel;
};


class Linear
{
    private:
        double mGradient, mIntercept, mResidual;
        int mDataNum;
        std::vector<std::vector<double>> mDatas;
        std::vector<int> mInliers;

    public:
        Linear() = default;
        Linear(const std::vector<std::vector<double>>& datas, const int& dataNum);
        void run(std::mt19937& gen, const double& margin);
        std::tuple<double,double> computeLinearParameter(const double& x1, const double& y1, const double& x2, const double& y2);
        std::tuple<double, double> getParameter();
        double getResidual();
        std::vector<int> getInliers();
};
