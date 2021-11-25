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

Logic behind the nodes
----------------------

To satisfy the requests I decided to code three different nodes inside the package, more the one regarding the stage_ros simulator. Here's the idea behind the communication of the nodes:
