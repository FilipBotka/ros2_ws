#pragma once

#include "matplotlibcpp.h"
#include <vector>
#include <memory>
#include <cmath>
#include <iostream>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

namespace plt = matplotlibcpp;

class LidarData : public rclcpp::Node
{   

    public:
        LidarData();
    

    private:
        void laserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;

};