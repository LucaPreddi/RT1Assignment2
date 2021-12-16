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
		req.input = 'g';
	}

	// If the input is 's' we decrease the value.

	if(req.input == 's'){
		stuff -= 0.5;
		req.input = 'g';
	}

	// If the input is 'r' we reset the position by calling the 
	// service /reset_positions.

	if(req.input == 'r'){
		ros::service::call("/reset_positions", res_server);
	}

	// We use x as input in order not to modify in loop the value
	// of the velocity.

	if(req.input == 'g'){
	}

	// We put in the response the value.

	res.val = stuff;   
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