# ROS
homework&amp;practice in ROS course

by BigGreenTW

---
## HW6
- my youtube
[![Watch the video](https://img.youtube.com/vi/cV2PwPAWOXM/maxresdefault.jpg)](https://youtu.be/cV2PwPAWOXM)

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
    (C++ templates makes automatic differentiation efficient)
```c++=
Problem problem;
CostFunction* cost_function =
      new AutoDiffCostFunction<CostFunctor, 1, 1>(new CostFunctor);
  problem.AddResidualBlock(cost_function, NULL, &x);
```
for NumericDiffCostFunction
(numeric differentiation is expensive, prone to numeric errors, and leads to slower convergence.)
```c++=
Problem problem;
CostFunction* cost_function =
  new NumericDiffCostFunction<NumericDiffCostFunctor, ceres::CENTRAL, 1, 1>(new NumericDiffCostFunctor);
problem.AddResidualBlock(cost_function, NULL, &x);
```


  
  
