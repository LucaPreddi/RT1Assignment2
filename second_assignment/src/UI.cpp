#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerate.h"
#include "second_assignment/Accval.h"
#include "std_srvs/Empty.h"

// Declaring ServiceClient global object.

ros::ServiceClient client1;

// Declaring Publisher object.

ros::Publisher pub;

// Implementing the function to read the input char.

char GetChar(){
	char c;
	std::cout << "Do you want to accelarate or decelerate? [a] or [s]?\n";
	std::cout << "If you want to reset, press 'r'!\n";
	std::cin >> c;
	return c;
}

void CharCallback(){

	// Creating Accelerate object to send a request
	// to the server.

	second_assignment::Accelerate a_srv;

	// Reading the input.

	char in = GetChar();

	// Sending a char as request.

	a_srv.request.input = in;

	// Waiting for the server to give a response.

	client1.waitForExistence();
	client1.call(a_srv);

	// Creating a message to send the response to
	// controller.cpp.

	second_assignment::Accval n;
	n.acc = a_srv.response.val;
	pub.publish(n);
}

int main (int argc, char **argv) 
{
	// Initialize the node, setup the NodeHandle 
	// for handling the communication with the 
	// ROS //system.

	ros::init(argc, argv, "UI"); 
	ros::NodeHandle nh;

	// Defining the publishing of the message Accval.

	pub = nh.advertise<second_assignment::Accval>("/accval", 10); 

	// Creating the client with his attributes.

	client1 = nh.serviceClient<second_assignment::Accelerate>("/accelarate");

	// Making the function spin.

	while(ros::ok()){
		CharCallback();
		ros::spinOnce();
	}

	return 0;
}