#!/bin/bash
export TURTLEBOT_GAZEBO_MAP_FILE="$(pwd)/../map/map.yaml" 
xterm -e " roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(pwd)/../map/parkingCar.world" &
sleep 5
xterm -e " roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$TURTLEBOT_GAZEBO_MAP_FILE initial_pose_a:=180" &
sleep 5
xterm -e " roslaunch turtlebot_rviz_launchers view_navigation.launch" & 
echo $TURTLEBOT_GAZEBO_MAP_FILE 
sleep 5
xterm -e " rosrun pick_objects pick_objects" &
xterm -e " rosrun add_markers addMarkers"
