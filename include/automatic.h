#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "matplotlibcpp.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int16.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

namespace plt = matplotlibcpp;

class Automatic : public rclcpp::Node
{
    public:
        Automatic();

        void laserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        void masterCallback(const std_msgs::msg::Int16::SharedPtr msg);
        void cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr getPublisher() const;

        int state_;
        double vel_linear_;
        double vel_angular_;

    private:
        
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;
        rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr sub_master_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_vel_;

        



};

