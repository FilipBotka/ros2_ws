#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "std_msgs/msg/int16.hpp"

class Master : public rclcpp::Node 
{
    public:
        Master();

        void getInput();


    private:
        rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_;


};