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
