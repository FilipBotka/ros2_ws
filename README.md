# Kobuki Robot Controller - ROS2/C++

This ROS2 package, uds_kobuki, provides a set of nodes to control the Kobuki robot. It currently includes three nodes: master, pub, and process.
## Nodes
### Master Node

The master node serves as a command center, allowing you to enable or disable manual control of the robot. You can activate manual control by entering 1 in the console; this permits the pub node to publish your commands to the robot. Any other number will cease the robot's movement.

### Publisher Node

The pub node allows you to steer the robot using arrow keys on your keyboard, but it requires manual control to be enabled by the master node first.

### Process Node

The process node handles inputs from the pub node and monitors the state to determine if it's permissible to publish commands to the robot.
Running the Nodes

## Running the Nodes

To operate the nodes in the uds_kobuki package, you'll need to follow these steps:

go to your `ros2_ws` and do `colcon build`.

first you have to run `ROS on kobuki robot`
```sh
ros2 run uds_kobuki_ros uds_kobuki_ros
```
Then we need to start the master node which will handle the input commands:
```sh
ros2 run uds_kobuki master
```
Then process node which handles the communication between master and manual mode:
```sh
ros2 run uds_kobuki prcs
```
Then node which handels the manual input 
```sh
ros2 run uds_kobuki pub_cmd
```
Now the last node, which handels automatic mode:
```sh
ros2 run uds_kobuki automatic
```




process node is just handling your inputs from pub node and checking the state if it can publish to robot
