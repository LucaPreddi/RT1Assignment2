#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerate.h"
#include "second_assignment/Accval.h"
#include "std_srvs/Empty.h"

// Declaring the publisher and the message, plus some global variables.

ros::Publisher pub;
geometry_msgs::Twist my_vel;
float acc = 0;
float adder = 0;

// Developing the StudyDistance() function, we want in input an array of floats 
// and the indexes where we want the function to iterate the for loop().

float StudyDistance(int min, int max,float ranges[]){

    float val_min = 100.0;
    for(int i = min; i <= max; i++){
        if (ranges[i]<=val_min) val_min = ranges[i];
    }
    return val_min;
}

// Developing the RobotCallback() function, where the control of the 
// robot is managed.

void RobotCallback(const sensor_msgs::LaserScan::ConstPtr& msg){

    // Declaring some local variables.

    float val_min_right, val_min_front, val_min_left;
    float laser[721];
    
    // For loop() to fill an array which will be passed to StudyDistance()
    // function, 

    for(int i = 0; i<721; i++){
        laser[i] = msg->ranges[i];
    }

    // Studying the closest distance on the right, front and left.

    val_min_right = StudyDistance(0, 100, laser); 
    val_min_front = StudyDistance(310, 410, laser); 
    val_min_left = StudyDistance(620, 720, laser);

    // If the value on the front is lower than 1.5 we want our robot 
    // to turn either left or right. This is really important for the
    // relationship between the robot and the walls.

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

    // If we are far away from the walls, we go straght.

    else{
        my_vel.linear.x = 1 + adder;
        my_vel.angular.z = 0;
    }

    // Publishing the velocity of the robot.

    float velocity = my_vel.linear.x;
    ROS_INFO("Right: @[%f]", adder);    
    pub.publish(my_vel);
}

// The AccelarationCallback is crucial for the code because thanks to the 
// custom message Accval we can easily get the velocity from the UI node.

void AccelarationCallback(const second_assignment::Accval::ConstPtr& uno){
    adder = uno->acc;
}

int main (int argc, char **argv)
{
    // Initializing the node, setup the NodeHandle.

    ros::init(argc, argv, "control");  
    ros::NodeHandle nh;

    // Defining the publisher.

    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);

    // Declaring first and second suscriber, to accbal topic and to base_scan.

    ros::Subscriber sub2 = nh.subscribe("/accval", 1, AccelarationCallback);
    ros::Subscriber sub = nh.subscribe("/base_scan", 1, RobotCallback);

    // Spinning the callback function.

    ros::spin();
    return 0;
}