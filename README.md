# RT1Assignment2
The second assignment of the Research Track 1 class regards the use of a robot simulator in ROS (Robot Operating System) and the programming of a really simple UI (User Interface).

Installing and running
----------------------

The simulator requires ROS (Robot Operating System), which is a set of software libraries and tools that help you build robot applications. The simulator perfectly runs on the [Noetic Release of ROS](http://wiki.ros.org/noetic/Installation), I haven't tried the [ROS 2 release](https://docs.ros.org) yet, so let me know if it does actually work. 

Anyway you can check every release of ROS in this [link](http://wiki.ros.org/ROS/Installation).

Once you have installed and ran (with ```$ roscore```) ROS, you should've even created a workspace where you can build up your packages. So, if you haven't still done it, download the package on GitHub and copy it in your `/src` folder. Then you should run:
```bash
$ catkin_make
```
to make ROS understand your package.

Once you have installed ROS and the package, run in five different consoles the following commands:
```bash
$ roscore &
```
```bash
$ rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world
```
```bash
$ rosrun second_assignment controller_node
```
```bash
$ rosrun second_assignment server_node
```
```bash
$ rosrun second_assignment UI_node
```
(__small tip, use [Terminator](https://dev.to/xeroxism/how-to-install-terminator-a-linux-terminal-emulator-on-steroids-1m3h) emulator, pretty useful when you need to use many consoles at the same time!__) 

<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/Terminator.png" width="400" height="300">
</p>

Introduction
----------------------

The aim of the project was to make the robot move inside a circuit (actually, to be more accurate, the Monza Circuit) without letting it hit the walls and, thanks to a UI, making the user decide the speed of the robot.

The circuit is the following:
<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/MonzaCircuit.png" width="500" height="450">
</p>

Yes, that small dot on the left is the actual robot, which is moving clockwise. The professor had many request about the assignment:
- The node controlling the robot should also implement the functionality to increase and decrease velocity through the use of a server.
- The Ul node should constantly wait for an input for the user, which can either ask to increment or decrement the velocity, or to put the robot to an initial state. The commands are:
  - 'a' to accelerate.
  - 'b' to decelerate.
  - 'r' to reset the position of the robot.

Logic behind the code
----------------------

To satisfy the requests I decided to code three different nodes inside the package, more the one regarding the stage_ros simulator. Here's the idea behind the communication of the nodes:

<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/Blank%20diagram.jpeg" width="520" height="360">
</p>

As you can see, it's not a tricky communication but it still needs some attention especially for the use of the server, I will explain each node by the tasks and the code inside of it.

_Briefly description_

In simple terms, __the user will give to the UI node an input__ that will be either a good one ('a', 's', 'r') or a bad one, which will give back an error input on the console. __The server will process this input, by releasing a float value that will be the acceleration value.__ Then __the UI will write in a custom message to the controller node which will read the actual value of the acceleration.__ Then __the controller node will send to the stageros node the updated informations of the velocity.__ __Another task of the server node is the one regarding the call__ of a service of the ROS library __of the reset of the position of the robot.__

## Nodes and their logic

_Here I will explain each node code and tasks, to have a deeper description of the code, check the comments inside of it._

### stageros node (stage_ros package)

The stageros node wraps the Stage 2-D multi-robot simulator, via libstage. Stage simulates a world as defined in a .world file. This file tells stage everything about the world, from obstacles (usually represented via a bitmap to be used as a kind of background), to robots and other objects.
The nodes has the following aspects:
- Subscriptions
  - The node subscribes to the `cmd_vel (geometry_msgs/Twist)` topic, to manage the velocity of the robot.
- Publishing
    - `odom (nav_msgs/Odometry)`: odometry data of the model. 
    - `base_scan (sensor_msgs/LaserScan)`: __scans from the laser model. We will use this one.__
    - `base_pose_ground_truth (nav_msgs/Odometry)`: __the ground truth pose. We will use this one.__
    - `image (sensor_msgs/Image)`: visual camera image.
    - `depth (sensor_msgs/Image)`: depth camera image.
    - `camera_info (sensor_msgs/CameraInfo)`: camera calibration info.

### controller node (second_assignment package)

The controller node has two main tasks, the first is to move the robot inside the enviroment se second changes the velocity of the robot. These tasks are completed with the callback function and the main of the node. The `main()` initialise the node with the init() function and subscribes to different topics.
- Subscriptions
  - `base_scan (sensor_msgs/Laser_scan)` which provides data about the laser that scans the surroundings of the robot. I will develop the laser code later.
  - `Accval (second_assignment/Accval)` which provides the amount to add to the base velocity.
- Publishing
  - `cmd_vel geometry_msgs/Twist` which is the topic to change the linear and angular velocity's values of the robot.

The main role of the controller node, as I said, is to make the robot move inside the enviroment, this happens thanks to the subscription to `base_scan`, such that the `RobotCallback()` function can loop with `ros::spin()` inside the `main()` function. 

After making the `RobotCallback()` function loop, I developed a simple algorithm made out of three _if_ statements. This algorithm is really close to the [assigment 1 `main()` function](https://github.com/LucaPreddi/RT1Assignment1#main-function-). This algorithm uses the `StudyDistance()` function which is super useful to get the distance of the nearest wall on the front, left and right of the robot. Here's the code:

```cpp
float StudyDistance(int min, int max,float ranges[]){

    float val_min = 100.0;
    for(int i = min; i <= max; i++){
        if (ranges[i]<=val_min) val_min = ranges[i];
    }
    return val_min;
}
```
Here's the idea behind the movement, the flowchart is really really simple because I used the old algorith used for the first assignment in python and it does work perfectly. Anyway the code can be improved by paying more attention to the parameters and if statements.

<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/Flowchar.jpeg" width="600" height="500">
</p>

With these informations, the algorithm can work perfectly.
Here in code (this is all inside the `RobotCallback()`):
```cpp
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
```
As you can see the code is really simple and it can be improved! But as far as I wanted to understand how ROS works, I preferred to spend time in improving the nodes relations instead of improving the algorithm itself. 

Another important function is the `AccelerationCallback()` which is the one that reads from the custom message the amount of velocity we want to add (called improprierly acceleration).
```cpp
void AccelarationCallback(const second_assignment::Accval::ConstPtr& uno){
    adder = uno->acc;
}
```
Where adder is a global variable that we use in the `RobotCallback()` function. 

### server node (second_assignment package)

This node is crucial for the use of the UI node and therefore therefore the use of the User Interface commands. As we saw before, the user can either press:
- 'a' to accelerate.
- 's' to decelerate.
- 'r' to reset the position of the robot inside the circuit.
To make the code fancier and to manage the input request and response in a better way. I could easily do it with two nodes.

Anyway when the user puts a variable inside the server from the UI node the server reads it and depending on wheter the input is it gives something in the response as output. The .srv file is structured like this:
```
char input
---
float32 val
```
as we can see there's a char in _request_ and a val in _response_. The value depends on the input obviously and it is assigned inside the ` ServerCallback()` function. Which is spinned on the main when the server service receives a request. Here in code:
```cpp
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
	}
	if(req.input != 'x' && req.input != 's' && req.input != 'a' && req.input != 'r'){
		std::cout << "It's not the right key!\n";
	}
	res.val = stuff;
	ROS_INFO("Right: @[%f]", res.val);    
	return true;
}
```
The char 'x' is just something to don't let the function increment endlessy the value. As we can see when we get as request 'r' we call the service `/reset_positions` of `res_server` to reset the position of the robot.

### UI node (second_assignment package)
The UI node as the name can suggest, is the core of the User Interface and it communicates with both the nodes, server and controller. It get the input by the terminal and sends a request to the server, which will provide a response still to the UI node. This happens all inside the CharCallback() function.
- Subscriptions
  - None.
- Publishing
  - The node publishes to the `Accval (second_assignment/Accval)` topic that controller.cpp will read.
Here's the code of the `CharCallback()` function:
```cpp
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
```
The function spins on the `main()` function as long as ROS is running with a `while(ROS::ok())` loop.
In code:
```cpp
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
```
So the UI node get the input, send a request to the server, waits for a response from the server, then sends the acceleration value to the controller node. In logic blocks:

<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/Schermata%202021-11-29%20alle%2015.59.31.png"width="450" height="159">
</p>

As we can see the UI node is the center of this communication.

## Conclusion and possible improvements
The work overall satisfied me especially because this is the first real kind of project I've ever approached with ROS. I understood the potentional of this tool and what can be easily created with it. To conclude the communication of the nodes I will show yo with the node

`rosrun rqt_graph rqt_graph`

the relationship between all the nodes:

<p align="center">
<img src="https://github.com/LucaPreddi/RT1Assignment2/blob/main/Images/Schermata%202021-12-12%20alle%2013.12.58.png">
</p>

The possible improvements that can be done are a lot. 
- First of all the algorithm managing the direction and speed of the robot can be improved because, as you can see when running, the robot has difficult when turning in the circuit. 
- Second, the UI interface has simple outputs and visuals for the user. If the UI node had more aesthetic values it can be easier to use and appreciatable.
