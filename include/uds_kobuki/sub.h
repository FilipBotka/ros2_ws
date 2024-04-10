#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "nav_msgs/msg/odometry.hpp"

class Odometry : public rclcpp::Node
{
    public:
        Odometry();

    private:
        void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg);
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscriber_;

};
