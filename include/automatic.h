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

        int state_;


    private:
        
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;
        rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr sub_master_;


};

