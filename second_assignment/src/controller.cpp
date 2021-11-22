#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerate.h"
#include "second_assignment/Accval.h"
#include "std_srvs/Empty.h"

ros::Publisher pub;
geometry_msgs::Twist my_vel;
float acc = 0;
float adder = 0;

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

    if(val_min_front<1.5){
        if(val_min_left>val_min_right){
            my_vel.angular.z = 1;
            my_vel.linear.x = 0.1;
        }
        else if(val_min_right>val_min_left){
            my_vel.angular.z = -1;
            my_vel.linear.x = 0.1;
        }
    }
    else{
        my_vel.linear.x = 1 + adder;
        my_vel.angular.z = 0;
    }
    float velocity = my_vel.linear.x;
    ROS_INFO("Right: @[%f]", acc);    
    pub.publish(my_vel);
}

void AccelarationCallback(const second_assignment::Accval::ConstPtr& uno){
    adder = uno->acc;
}

int main (int argc, char **argv)
{
// Initialize the node, setup the NodeHandle for handling the communication with the ROS //system  
    ros::init(argc, argv, "control");  
    ros::NodeHandle nh;
    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);   
    ros::Subscriber sub2 = nh.subscribe("/accval", 1, AccelarationCallback);
    ros::Subscriber sub = nh.subscribe("/base_scan", 1, RobotCallback);
    ros::spin();
    return 0;
}