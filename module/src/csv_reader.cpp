#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "eigen-3.4.0/Eigen/Dense"

void readCsv(const std::string& path, std::vector<std::vector<double>>& values, const int& cols)
{
    std::ifstream in(path);
    std::string s;
    const char& chr = ',';
    int row = 0, col = 0;

    if (in.is_open())
    {
        while(std::getline(in,s))
        {
            std::stringstream line(s);

            while(std::getline(line,s,chr))
            {
                values[row][col] = std::stod(s);
                col = (col+1) % cols;
                if(col == 0)
                {
                    ++row;
                }
            }


        }
    }
    else
    {
        std::cerr << "No file!" << std::endl;
    }

}

