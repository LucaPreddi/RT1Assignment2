#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerate.h"
#include "second_assignment/Accval.h"
#include "std_srvs/Empty.h"

ros::ServiceClient client1;
ros::Publisher pub;

char GetChar(){
	char c;
	std::cout << "Do you want to accelarate or decelerate? [a] or [s]?\n";
	std::cin >> c;
	return c;
}

void CharCallback(const sensor_msgs::LaserScan::ConstPtr& msg){
	second_assignment::Accelerate a_srv;
	char in = GetChar();
	a_srv.request.input = in;
	client1.waitForExistence();
	client1.call(a_srv);
	second_assignment::Accval n;
	n.acc = a_srv.response.val;
	pub.publish(n);
}

int main (int argc, char **argv) 
{
	// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system
	ros::init(argc, argv, "UI"); 
	ros::NodeHandle nh;
	// Define the subscriber to turtle's position
	pub = nh.advertise<second_assignment::Accval>("/accval", 10);   
	client1 = nh.serviceClient<second_assignment::Accelerate>("/accelarate");
	ros::Subscriber sub = nh.subscribe("/base_scan", 1, CharCallback);
	ros::spin();
	return 0;
}