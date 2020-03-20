// include ros library
#include "ros/ros.h"

// include msg library
#include <geometry_msgs/Twist.h>

// include cpp library 
#include <cstdio>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
//#include <>
geometry_msgs::Twist vel_msg;
/*
char getch()
{
  int flags = fcntl(0, F_GETFL, 0);
  fcntl(0, F_SETFL, flags | O_NONBLOCK);

  char buf = 0;
  struct termios old = {0};
  if (tcgetattr(0, &old) < 0) {
      perror("tcsetattr()");
  }
  old.c_lflag &= ~ICANON;
  old.c_lflag &= ~ECHO;
  old.c_cc[VMIN] = 1;
  old.c_cc[VTIME] = 0;
  if (tcsetattr(0, TCSANOW, &old) < 0) {
      perror("tcsetattr ICANON");
  }
  if (read(0, &buf, 1) < 0) {
      //perror ("read()");
  }
  old.c_lflag |= ICANON;
  old.c_lflag |= ECHO;
  if (tcsetattr(0, TCSADRAIN, &old) < 0) {
      perror ("tcsetattr ~ICANON");
  }
  return (buf);
}
*/
int getch()
{
  static struct termios oldt, newt;
  tcgetattr( STDIN_FILENO, &oldt);           // save old settings
  newt = oldt;
  newt.c_lflag &= ~(ICANON);                 // disable buffering
  tcsetattr( STDIN_FILENO, TCSANOW, &newt);  // apply new settings

  int c = getchar();  // read character (non-blocking)

  tcsetattr( STDIN_FILENO, TCSANOW, &oldt);  // restore old settings
  return c;
}
void KeyboardControl()
{
	
	//vel_msg.linear.x=0;
	vel_msg.angular.z=0;
	//int c = getch();
	char c;
	for(;;){
	if(read(0,&c,1)<0){
	perror("read():");
	exit(-1);
	}
	}
	//read(0,&c,1);
  /*
  if (vel_msg.angular.z>0)
	  vel_msg.angular.z --;
  else if(vel_msg.angular.z<0)
	  vel_msg.angular.z ++;
  if (vel_msg.linear.x>0)
          vel_msg.linear.x --;
  else if(vel_msg.linear.x<0)
          vel_msg.linear.x ++;
	  */
  if (c != EOF)
  {
	std::cout<< "c variable"<< c <<std::endl;
	/*Please input your codes here*/
	switch(c){
		case 'a':
			if(vel_msg.angular.z<21)
				vel_msg.angular.z =3;
			break;
		case 'd':
			if(vel_msg.angular.z>-21)
				vel_msg.angular.z =-3;
			break;
		case 'w':
			if(vel_msg.linear.x<21)
				vel_msg.linear.x =5;
			break;
		case 's':
			if(vel_msg.linear.x>-21)
				vel_msg.linear.x =-5;
			break;
		/*default:
			//vel_msg.linear.x=0;
			//vel_msg.angular.z=0;
			if (vel_msg.angular.z>0)
				vel_msg.angular.z --;
  			else if(vel_msg.angular.z<0)
          			vel_msg.angular.z ++;
  			
			if (vel_msg.linear.x>0)
          			vel_msg.linear.x --;
  			else if(vel_msg.linear.x<0)
          			vel_msg.linear.x ++;
			break;*/	
	}
			/*if (vel_msg.angular.z>0)
                                vel_msg.angular.z --;
                        else if(vel_msg.angular.z<0)
                                vel_msg.angular.z ++;

                        if (vel_msg.linear.x>0)
                                vel_msg.linear.x --;
                        else if(vel_msg.linear.x<0)
                                vel_msg.linear.x ++;*/
		//vel_msg.linear.x=0;
		//vel_msg.angular.z=0;	
  }/*else{
	  if (vel_msg.angular.z>0)
		  vel_msg.angular.z --;
	  else if(vel_msg.angular.z<0)
		  vel_msg.angular.z ++;
	  if (vel_msg.linear.x>0)
		  vel_msg.linear.x --;
	  else if(vel_msg.linear.x<0)
		  vel_msg.linear.x ++;
  }*/
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "tutorial_1");
  ros::NodeHandle n;

  // declare publisher
  ros::Publisher turtlesim_pub = n.advertise<geometry_msgs::Twist>("/turtle1/cmd_vel", 100);

  // setting frequency as 100 Hz
  ros::Rate loop_rate(100);

  printf("KeyboardControl start\n");

  int count = 0;
  int cc=0;
  while (ros::ok()){
	  KeyboardControl();

    turtlesim_pub.publish(vel_msg);
	std::cout<<"count :"<< count <<std::endl;
	std::cout<<"linear :"<< vel_msg.linear.x <<std::endl;
	std::cout<<"angular :"<< vel_msg.angular.z <<std::endl;

    count ++;
    cc ++;
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}



