# ROS
homework&amp;practice in ROS course

by BigGreenTW

---
## HW6
- my youtube
[![Watch the video](https://img.youtube.com/vi/cV2PwPAWOXM/maxresdefault.jpg)](https://youtu.be/cV2PwPAWOXM)

---
## HW7
- Q1:Transformation AB is rotated by 90 degree about the z axis and transformation BC is rotated by 60 degree about the z axis, and generate the output transformation AC.
![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW7_P1-2.jpg)
![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW7_P1.JPG)

- Q2:Transformation BA and CA are the inputs to generate output transformation BC.
![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW7_P2-2.jpg)
![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW7_P2.JPG)
---
## HW8
- cere_solver_tutorial 1
  + Question1: Know how to set resdiual.
    If a functor is f(x)=10-X, we will set rediual to
```c++=
residual[0] = T(10.0) - x[0];
```
if functor is f(x)=y-exp(m*x+c), we will set rediual to
```c++=
residual[0] = T(y_) - exp(m[0] * T(x_) + c[0]);
```
  + Question2:Know how different parts are between AutoDiffCostrFunction, NumericDiffCostFunction and AutoDiffCostrFunction.
    
for AutoDiffCostrFunction
**(C++ templates makes automatic differentiation efficient)**
```c++=
Problem problem;
CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
  problem.AddResidualBlock(cost_function, NULL, &x);
```
for NumericDiffCostFunction
**(numeric differentiation is expensive, prone to numeric errors, and leads to slower convergence.)**
```c++=
Problem problem;
CostFunction* cost_function =
  new NumericDiffCostFunction<NumericDiffCostFunctor, ceres::CENTRAL, 1, 1>(new NumericDiffCostFunctor);
problem.AddResidualBlock(cost_function, NULL, &x);
```

  + Question3: Will it affect the whole iteration if knowing an iteration is longer than the other ?
  
  No, it won't affect the whole iteration.
  
  ![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW8_p1.JPG)

- cere_solver_tutorial 2
+ 1.include some package
```c++=
#include <ros/ros.h>
#include <ceres/ceres.h>
#include <ceres/rotation.h>
#include <eigen3/Eigen/Dense>
#include <queue>
```
+ 2.write a functor

![Image description](https://github.com/biggreentw/ROS/blob/master/source/HW8_p2.PNG)
```c++=
// f1 = x1 + 10 * x2
struct F1 {
  template <typename T>
  bool operator()(const T* const x1, const T* const x2, T* residual) const {
    residual[0] = (x1[0] + T(10.0) * x2[0]);
    return true;
  }
};

// f2 = 5^0.5 * (x3 - x4)
struct F2 {
  template <typename T>
  bool operator()(const T* const x3, const T* const x4, T* residual) const {
    residual[0] = T(sqrt(5.0)) * (x3[0] - x4[0]);
    return true;
  }
};

// f3 = (x2 - 2 * x3)^2
struct F3 {
  template <typename T>
  bool operator()(const T* const x2, const T* const x3, T* residual) const {
    residual[0] = (x2[0] - T(2) * x3[0]) * (x2[0] - T(2) * x3[0]);
    return true;
  }
};

// f4 = 10^0.5 * (x1 - x4)^2
struct F4 {
  template <typename T>
  bool operator()(const T* const x1, const T* const x4, T* residual) const {
    residual[0] = T(sqrt(10.0)) * (x1[0] - x4[0]) * (x1[0] - x4[0]);
    return true;
  }
};
```
+ 3.new costFunction struct, here have 4 different functor
```c++=
    double x1, x2, x3, x4;
    x1 = 1;
    x2 = 2;
    x3 = 5;
    x4 = 6;
    ceres::Problem problem;
    problem.AddResidualBlock(
      new ceres::AutoDiffCostFunction<F1, 1, 1, 1>(new F1), NULL, &x1, &x2);
      //<F1, 1, 1, 1>  ------------->  <struct of functor, dimension of x1, dimension of x2, dimension of residual>
      //NULL -----------------------> means that do not use loss_function
      //&x1, &x2 -------------------> solve to find the x1, x2
      //AddResidualBlock(CostFunction *cost_function, LossFunction *loss_function, double *x0, double *x1, ...)
    problem.AddResidualBlock(
      new ceres::AutoDiffCostFunction<F2, 1, 1, 1>(new F2), NULL, &x3, &x4);
    problem.AddResidualBlock(
      new ceres::AutoDiffCostFunction<F3, 1, 1, 1>(new F3), NULL, &x2, &x3);
    problem.AddResidualBlock(
      new ceres::AutoDiffCostFunction<F4, 1, 1, 1>(new F4), NULL, &x1, &x4);
```
+ 4.Run the solver. 
```c++=
    //Create the "Optine"
    ceres::Solver::Options options;
    
    //Set some about "Optine"
    options.linear_solver_type = ceres::DENSE_QR;
    options.minimizer_progress_to_stdout = true;
    
    //Create the "Summary"
    ceres::Solver::Summary summary;
    
    //Solve, then get the answer.
    ceres::Solve(options, &problem, &summary);
    
    std::cout << "x : " << x1 << " " << x2 << " " << x3 << " " << x4 << std::endl;
}
```
  
