#include "second_assignment/Accelerate.h"
#include "ros/ros.h"
#include "std_srvs/Empty.h"

// Initializing an object Empty.

std_srvs::Empty res_server;

float stuff = 0;
	
// Developing ServerCallback() function, the function dedicated to
// chaneg the value of the velocity by communicating with UI node.

bool ServerCallback(second_assignment::Accelerate::Request &req, second_assignment::Accelerate::Response &res){

	// If the input is 'a' we increase the value.

	if(req.input == 'a'){
		stuff += 0.5;
		req.input = 'x';
	}

	// If the input is 's' we decrease the value.

	if(req.input == 's'){
		stuff -= 0.5;
		req.input = 'x';
	}

	// If the input is 'r' we reset the position by calling the 
	// service /reset_positions.

	if(req.input == 'r'){
		ros::service::call("/reset_positions", res_server);
	}

	// We use x as input in order not to modify in loop the value
	// of the velocity.

	if(req.input == 'x'){
	}

	// If we put something else we print in the server node console
	// that the user has made some mistakes in typing.

	if(req.input != 'x' && req.input != 's' && req.input != 'a' && req.input != 'r'){
		std::cout << "It's not the right key!\n";
	}

	// We put in the response the value.

	res.val = stuff;
	ROS_INFO("Right: @[%f]", res.val);    
	return true;
}

int main(int argc, char **argv){

	// Initializing the node.

	ros::init(argc, argv, "server");
	ros::NodeHandle nh;

	// Declaring the service /accelarate.

	ros::ServiceServer service =  nh.advertiseService("/accelarate", ServerCallback);

	// Spinning the callback function.
	
	ros::spin();
	return 0;
}