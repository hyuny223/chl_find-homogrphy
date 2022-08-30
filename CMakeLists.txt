cmake_minimum_required(VERSION 3.5)
project(svd LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

link_libraries(stdc++fs)

find_package(OpenCV 4.5.5 REQUIRED)
find_package(Eigen3 3.4.0 REQUIRED)

add_subdirectory(module)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
        module/include
        ${OpenCV_INCLUDE_DIRS}
        ${Eigen_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} PUBLIC
        module
        ${OpenCV_LIBS}
        Eigen3::Eigen
        )
