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
  double ex[3]={0,0,0};
  double ey[3]={0,0,0};
  double ox=0;
  double oy=0;
  double total_rot=0;
  vel_msg.angular.z =0;
  vel_msg.linear.x=0;
  turtlesim_pub.publish(vel_msg);
  while (ros::ok()){

    printf("\ncount : %d\n",count);
    printf("goal x : %f \t y : %f\n",goal_point.x,goal_point.y);
    printf("pose x : %f \t y : %f\n",pose.x,pose.y);

    // Calculate position error(feedback term)
    double pos_err_x = goal_point.x - pose.x;
    double pos_err_y = goal_point.y - pose.y;
    printf("pos_errx : %f \t pos_erry: %f\n",pos_err_x,pos_err_y);


    ex[0]=ex[1];printf("ex0: %f \t",ex[0]);
    ex[1]=ex[2];printf("ex1: %f \t",ex[1]);
    ex[2]=pos_err_x*0.001-2*ex[1]-1*ex[0];printf("ex2: %f \n",ex[2]);
    ox=ex[0]+pose.x;

    ey[0]=ey[1];printf("ey0: %f \t",ey[0]);
    ey[1]=ey[2];printf("ey1: %f \t",ey[1]);
    ey[2]=pos_err_y*0.001-2*ey[1]-1*ey[0];printf("ey2: %f \n",ey[2]);
    oy=ey[0]+pose.y;
	printf("o x : %f \t y : %f\n",ox,oy);

    double dx=goal_point.x-ox; printf("dx : %f\n",dx);
    double dy=goal_point.y-oy; printf("dy : %f\n",dy);
    double dis=sqrt(pow(dx,2)+pow(dy,2)); printf("dis : %f\n",dis);
    double angle;
    if (dis==0){
     angle=0; printf("angle : %f\n",angle);
    }else{
     angle=acos(dy/dis); printf("angle : %f\n",angle);
    }
    	//angle=angle%(2*3.1415926);

    double vel=dis/0.1; printf("vel : %f\n",vel);

    /*Your error-driven controller design
     *-->
     *
     *
     *
     *
     */
    vel_msg.linear.x=vel;
    if(vel<10){
    	vel_msg.linear.x = vel/10;
    }else{
	//vel_msg.linear.x=10;
	vel_msg.linear.x=vel/10;
    }
    printf("pose.theta: %f \n",pose.theta);
    printf("total_ror: %f \n",total_rot);
    double vel_a=(-(total_rot-angle))*10.0;
    printf("vel_a: %f \n",vel_a);
    /*
    if(vel_a>20){
	    vel_a=20;
    }else if(vel_a<-20){
    		vel_a=-20;
    }*/
    vel_msg.angular.z =vel_a ;
    turtlesim_pub.publish(vel_msg);
    printf("vel linear : %f \t augular : %f\n",vel_msg.linear.x, vel_msg.angular.z);
    total_rot+=angle;
    if(total_rot>3.1415926){
    	total_rot-=3.1415926;
    }
    count ++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}




