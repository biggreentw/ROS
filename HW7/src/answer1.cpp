#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <Eigen/Geometry>
#include <eigen3/Eigen/Dense>

// Callback of timer
// Broadcaster transform from A to B and B to C
void broadcastTF(const ros::TimerEvent& timer_event)
{
  static tf::TransformBroadcaster br;

  //qCB
  Eigen::Quaterniond q_C_B(std::sqrt(3)/2, 0, 0, 0.5);
  Eigen::Vector3d v_C_B(1,0,0);

  tf::Transform tf_C_B;
  tf::Quaternion tf_q_C_B(q_C_B.x(),q_C_B.y(),q_C_B.z(),q_C_B.w());
  tf::Vector3 tf_v_C_B(v_C_B(0), v_C_B(1), v_C_B(2));

  tf_C_B.setOrigin(tf_v_C_B);
  tf_C_B.setRotation(tf_q_C_B);

  br.sendTransform(tf::StampedTransform(tf_C_B,
			  		ros::Time::now(),
					"C",
					"B"));

  //qBA
  Eigen::Quaterniond q_B_A(std::sqrt(2)/2, 0, 0, std::sqrt(2)/2);
  Eigen::Vector3d v_B_A(1,0,0);

  tf::Transform tf_B_A;
  tf::Quaternion tf_q_B_A(q_B_A.x(),q_B_A.y(),q_B_A.z(),q_B_A.w());
  tf::Vector3 tf_v_B_A(v_B_A(0), v_B_A(1), v_B_A(2));

  tf_B_A.setOrigin(tf_v_B_A);
  tf_B_A.setRotation(tf_q_B_A);

  br.sendTransform(tf::StampedTransform(tf_B_A,
			  		ros::Time::now(),
					"B",
					"A"));

  //qCA
  Eigen::Quaterniond q_C_A=q_C_B*q_B_A;
  //Eigen::Vector3d v_C_A=q_B_A.inverse()*(q_C_B.inverse()*v_C_B)+q_B_A.inverse()*v_B_A;
  
  Eigen::Vector3d v_C_A=v_C_B+q_C_B*v_B_A;
  
  //std::cout<<q_B_A*(q_C_B*v_C_B)<<std::endl;

  //std::cout<<q_C_B*v_B_A<<std::endl;
  //Eigen::Vector3d v_C_A(sqrt(3),sqrt(1),0);
  //v_C_A=v_C_A.normalized()*sqrt(3);
  
  tf::Transform tf_C_A;
  tf::Quaternion tf_q_C_A(q_C_A.x(),q_C_A.y(),q_C_A.z(),q_C_A.w());
  tf::Vector3 tf_v_C_A(v_C_A(0), v_C_A(1), v_C_A(2));

  tf_C_A.setOrigin(tf_v_C_A);
  tf_C_A.setRotation(tf_q_C_A);

  br.sendTransform(tf::StampedTransform(tf_C_A,
			  		ros::Time::now(),
					"C",
					"A1"));
}

int main(int argc, char** argv)
{
  ros::init(argc, argv, "ncrl_tf_learning");
  ros::NodeHandle nh;
  // Create timer with 2.0 Hz
  ros::Timer timer = nh.createTimer(ros::Duration(0.5), broadcastTF);
  while (ros::ok()){ros::spinOnce();}
  return 0;
}
