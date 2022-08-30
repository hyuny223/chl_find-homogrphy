# chl_find-homogrphy

# 1. Goal of Project
The goal of this project is to find homography matrix when datas were given using Eigen Library.  


# 2. How to find Homography
The way to find Homography is to decompose H matrix into singular value matrices when 3D points and 2D points are given. In this part, I used Eigen Library.  
After decomposing, find the one with the smallest singular value. Last one!  


# 3. How to run  
Below is the examples. But you should prepare cvs files. One is a data file in the form of [u, v, x, y, z] and the other is H matrix.  
```bash
$ mkdir build && cd build
$ cmake -G Ninja ..
$ ./svd ../data/{datafile}.csv ../data/{Hmatrixfile}.csv
```


# 4. Feature
In order to robust against outliers, I implemented RANSAC code.  


# 5. Result
## 1. RMSE
I used RMSE for evaluation  
```C++
while
{
    ...
    
    gt_error += std::sqrt((x - gt_x)*(x - gt_x) + (y - gt_x)*(y - gt_x));

    pred_error += std::sqrt((x - pred_x)*(x - pred_x) + (y - pred_y)*(y - pred_y));
}

gt_error /= len;
pred_error /= len;
```
RMSE of GT H Matrix : 63.7616  
RMSE of Predicted H Matrix : 63.55

## 2. Visualization
White : GT  
Red : Given H matrix  
Green : Predicted H matrix  
![image](https://user-images.githubusercontent.com/58837749/187387673-60e3c692-eaa2-4b8b-975f-7009852d532f.png)


# 6. Limitations
## 1. RANSAC
I made RANSAC only for linear regression. This is a big limitation for removing outliers. I need to implement RANSAC for polyfit, circular etc.
