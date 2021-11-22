#include "second_assignment/Accelerate.h"
#include "ros/ros.h"
#include "std_srvs/Empty.h"


std_srvs::Empty res_server;
float stuff = 0;

bool ServerCallback(second_assignment::Accelerate::Request &req, second_assignment::Accelerate::Response &res){
	if(req.input == 'a'){
		stuff += 0.5;
		req.input = 'x';
	}
	if(req.input == 's'){
		stuff -= 0.5;
		req.input = 'x';
	}
	if(req.input == 'r'){
		ros::service::call("/reset_positions", res_server);
	}
	if(req.input == 'x'){
		return false;
	}
	if(req.input != 'x' && req.input != 's' && req.input != 'a' && req.input != 'r'){
		std::cout << "It's not the right key!\n";
	}
	res.val = stuff;
	ROS_INFO("Right: @[%f]", res.val);    
	return true;
}

int main(int argc, char **argv){
	ros::init(argc, argv, "server");
	ros::NodeHandle nh;
	ros::ServiceServer service =  nh.advertiseService("/accelarate", ServerCallback);
	ros::spin();
	return 0;
}