#include "ros/ros.h"
#include <Eigen/Dense>
#include <cmath>

#define M_PI 3.141592653589

Eigen::Vector3d rad2deg(Eigen::Vector3d radians)
{
    // Implement your code here
    return radians=radians/M_PI*180;
}

Eigen::Vector3d deg2rad(Eigen::Vector3d degrees)
{
    // Implement your code here
    return degrees=degrees/180*M_PI;
}

Eigen::Quaterniond Euler2Quaternion(Eigen::Vector3d euler)
{
    Eigen::Quaterniond Q;
    Q = Eigen::AngleAxisd(euler.z(), Eigen::Vector3d::UnitZ()) *
        Eigen::AngleAxisd(euler.y(), Eigen::Vector3d::UnitY()) *
        Eigen::AngleAxisd(euler.x(), Eigen::Vector3d::UnitX());
    return Q;
}

Eigen::Vector3d Quaternion2Euler(Eigen::Quaterniond Q)
{
    Eigen::Vector3d Euler(0, 0, 0);
    Euler.x() = atan2(2 * (Q.w() * Q.x() + Q.y() * Q.z()), (1 - 2 * (Q.x() * Q.x() + Q.y() * Q.y())));
    // Implement your code here
    Euler.y() = asin(2 * (Q.w() * Q.y() - Q.x() * Q.z()));
    Euler.z() = atan2(2 * (Q.w() * Q.z() + Q.x() * Q.y()), (1 - 2 * (Q.z() * Q.z() + Q.y() * Q.y())));
    return Euler;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "eigen_transform");
    ros::NodeHandle nh;
    ros::Rate loop_rate(1);

    Eigen::Vector3d world_point(1, 0, 0);
    Eigen::Vector3d tmp_point = world_point;
    Eigen::Vector3d euler_angle_deg(0, 0, 90);
    // porpotion of euler_angle
    double s = 0.667;
    int count = 0;
    std::cout << "Current point position " << world_point.transpose() << std::endl << std::endl;

    while (ros::ok())
    {
        Eigen::Vector3d euler_angle_rad = deg2rad(euler_angle_deg);
        //std::cout << euler_angle_rad.transpose() << std::endl;

        Eigen::Quaterniond Q_Total = Euler2Quaternion(euler_angle_rad);
        //std::cout << Q_Total.coeffs().transpose() << std::endl;

        Eigen::Quaterniond Q_tmp = Eigen::Quaterniond::Identity().slerp(s, Q_Total);
        std::cout << Q_tmp.coeffs().transpose() << std::endl;

        Eigen::Vector3d euler_tmp_rad = Quaternion2Euler(Q_tmp);
        Eigen::Vector3d euler_tmp_deg = rad2deg(euler_tmp_rad);
        std::cout << "Apply rotation roll(X): " << euler_tmp_deg.x()
                  << ", pitch(Y): " << euler_tmp_deg.y()
                  << ", yaw(Z): " << euler_tmp_deg.z() << std::endl;

        // Apply rotation to point with quaternion
        // Implement your code here
	//world_point=Quaternion2Euler(Q_tmp*Euler2Quaternion(world_point)*(Q_tmp.inverse()));
        world_point=Q_tmp._transformVector(world_point);
	count ++;
        //std::cout << "Q_tmp:\n " << Q_tmp << std::endl << std::endl;
        std::cout << "Current point position " << world_point.transpose() << std::endl << std::endl;

        Eigen::Vector3d point_diff = world_point - tmp_point;
        if (point_diff.norm() < 0.1) {
            std::cout << "Current point rotate " << count << " times to origin position!" << std::endl;
            break;
        }
        loop_rate.sleep();
        ros::spinOnce();
    }
    return 0;
}
