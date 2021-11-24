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
