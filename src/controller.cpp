#include "ros/ros.h"
#include "turtlesim/Pose.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "turtlesim/Kill.h"
#include "turtlesim/TeleportAbsolute.h"

ros::Publisher pub;

float StudyDistance(int min, int max,float ranges[]){

    float val_min = 100.0;
    for(int i = min; i <= max; i++){
        if (ranges[i]<=val_min) val_min = ranges[i];
    }
    return val_min;
}

void RobotCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

    float val_min_right, val_min_front, val_min_left;
    float laser[721];
    for(int i = 0; i<721; i++){
        laser[i] = msg->ranges[i];
    }
    val_min_right = StudyDistance(0, 100, laser); 
    val_min_front = StudyDistance(310, 410, laser); 
    val_min_left = StudyDistance(620, 720, laser);

    ROS_INFO("Right: @[%f], Front: @[%f], left: @[%f]", val_min_right, val_min_front, val_min_left);
    geometry_msgs::Twist my_vel;
    my_vel.linear.x=1.0;
    pub.publish(my_vel);

}

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system  
 ros::init(argc, argv, "control");  
 ros::NodeHandle nh;
 pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);   

 ros::Subscriber sub = nh.subscribe("/base_scan", 1, RobotCallback);
 
 // Define the subscriber to turtle's position  
 //client_random=nh.serviceClient<my_srv::Velocity>("/velocity");
 //ros::ServiceClient client2 =  nh.serviceClient<turtlesim::Kill>("/kill");
 //turtlesim::Kill srv2;
 //srv2.request.name="turtle1";
 //client2.waitForExistence();
 //client2.call(srv2);
 
 
 /*ros::ServiceClient client1 =  nh.serviceClient<turtlesim::Spawn>("/spawn");
 turtlesim::Spawn srv1;
 srv1.request.x=1.0; 
 srv1.request.y=5.0;
 srv1.request.theta=0.0;
 srv1.request.name="rt1_turtle";
 client1.waitForExistence();
 client1.call(srv1);
 
 ros::ServiceClient client3 =  nh.serviceClient<turtlesim::TeleportAbsolute>("/rt1_turtle/teleport_absolute");
 turtlesim::TeleportAbsolute srv3;
 srv3.request.x=1.5;
 srv3.request.y=1.0;
 srv3.request.theta=0.0;
 client3.waitForExistence();
 client3.call(srv3);*/
 
 
   //check the topic name
  
 

 ros::spin();
 return 0;
}