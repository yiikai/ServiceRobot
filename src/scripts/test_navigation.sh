#!/bin/bash
export TURTLEBOT_GAZEBO_MAP_FILE="/home/robond/workspace/FinalProj/src/map/map.yaml" 
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/robond/workspace/FinalProj/src/map/parkingCar.world" &
sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" & 
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$TURTLEBOT_GAZEBO_MAP_FILE"
echo $TURTLEBOT_GAZEBO_MAP_FILE 
