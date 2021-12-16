#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerate.h"
#include "second_assignment/Accval.h"
#include "std_srvs/Empty.h"

// Defining colors.

#define BHBLK "\e[1;90m"
#define BHRED "\e[1;91m"
#define BHGRN "\e[1;92m"
#define BHYEL "\e[1;93m"
#define BHBLU "\e[1;94m"
#define BHMAG "\e[1;95m"
#define BHCYN "\e[1;96m"
#define BHWHT "\e[1;97m"
#define RESET "\033[0m"

// Declaring ServiceClient global object.

ros::ServiceClient client1;

// Declaring Publisher object.

ros::Publisher pub;

bool stop = false;

// Implementing the function to read the input char.

char GetChar(){
	char c;
	printf(BHGRN "Do you want to accelarate or decelerate? [a] or [s]?\n" RESET);
	printf(BHCYN "If you want to reset, press 'r'!\n" RESET);
	printf(BHYEL "If you want to quit, press 'q'.\n" RESET);
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

	// If input is q we quit the program and manage errors.

	if(in == 'q') stop = true;
	if(in != 'q' && in != 'a' && in != 's' && in != 'r') printf(BHRED "\nError, you pressed the wrong key!\n\n" RESET);

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

	while(ros::ok() && !stop){
		CharCallback();
		ros::spinOnce();
	}

	return 0;
}