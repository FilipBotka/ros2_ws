program in Ros2/C++ for controlling kobuki robot. It has for now 3 useful nodes: master, pub, process.

With master node you are controlling if you want to contorl robot with arrows or not.
If insert 1 to console pub node is free to publish your comands to robot. 
After you insert some other number the robot will stop.

With pub node you can control the robot with arrows(If you set state to 1 with master node)

process node is just handling your inputs from pub node and checking the state if it can publish to robot
