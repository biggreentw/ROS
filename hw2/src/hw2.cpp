// include ros library
#include "ros/ros.h"

// include msg library
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Point.h>

// include math 
#include <math.h>

using namespace std;

turtlesim::Pose pose;
geometry_msgs::Twist vel_msg;
geometry_msgs::Point goal_point;
//Define a data structure to 3D
struct XYZ{
  float x;
  float y;
  float z;
};
//Declare a variable.Its name is pos_err with XYZ data type
struct XYZ pos_err;

// declare call back function(call back the pose of robot)
void pos_cb(const turtlesim::Pose::ConstPtr& msg)
{
  pose = *msg;
} 

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tutorial_1");
  ros::NodeHandle n;

  // declare publisher & subscriber
  ros::Subscriber pos_sub = n.subscribe<turtlesim::Pose>("turtle1/pose", 10, pos_cb);
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);
  //input your desired position
  ROS_INFO("Please input (x,y). x>0,y>0");
  cout<<"desired_X:";
  cin>>goal_point.x;
  cout<<"desired_Y:";
  cin>>goal_point.y;
  // setting frequency as 10 Hz
  ros::Rate loop_rate(10);

  int count = 0;
  while (ros::ok()){

    printf("\ncount : %d\n",count);
    printf("goal x : %f \t y : %f\n",goal_point.x,goal_point.y);
    printf("pose x : %f \t y : %f\n",pose.x,pose.y);

    // Calculate position error(feedback term)
    double pos_err_x = goal_point.x - pose.x;
    double pos_err_y = goal_point.y - pose.y;
    printf("pos_errx : %f \t pos_erry: %f\n",pos_err_x,pos_err_y);
    double distance=sqrt(pow(pos_err_x,2)+pow(pos_err_y,2));
    printf("dis:%f \t",distance);
    double angle=atan2(pos_err_y,pos_err_x);
    printf("ang:%f \n",angle);

    vel_msg.linear.x=2*distance;
    vel_msg.angular.z=4*(angle-pose.theta);
    turtlesim_pub.publish(vel_msg);
    //if(distance<0.01)
	    //break;
    /*Your error-driven controller design
     *-->
     *
     *
     *
     *
     */
    count ++;
    ros::spinOnce();
    loop_rate.sleep();
  }
    vel_msg.linear.x=0;
    vel_msg.angular.z=0;
    turtlesim_pub.publish(vel_msg);


  return 0;
}




