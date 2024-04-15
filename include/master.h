#pragma once

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "std_msgs/msg/int16.hpp"
#include "geometry_msgs/msg/twist.hpp"

class Master : public rclcpp::Node 
{
    public:
        Master();

        void getInput();
        std_msgs::msg::Int16 getInput2();
        rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr getPublisher() const;

        void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg);
        void cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
        bool isZero();

    private:
        rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_;
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscriber_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_vel_;

        double vel_linear_;
        double vel_angular_;

};