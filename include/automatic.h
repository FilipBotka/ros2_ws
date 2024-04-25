#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <mutex>
#include <fstream>

#include "matplotlibcpp.h"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int16.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "nav_msgs/msg/odometry.hpp"

namespace plt = matplotlibcpp;

class Automatic : public rclcpp::Node
{
    public:
        Automatic();

        void laserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
        void masterCallback(const std_msgs::msg::Int16::SharedPtr msg);
        void cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
        void cmdPrcsCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
        void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg);

        void timerCallback();
        void timerPlotCallback();
        void timerAuto();

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr getPublisher() const;

        double vel_linear_;
        double vel_angular_;
        
        int state_;
        double distance_;
    private:
        
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscriber_;

        rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr sub_master_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_vel_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sub_prcs_;
        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_odom_;

        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::TimerBase::SharedPtr timer_plot_;
        rclcpp::TimerBase::SharedPtr timer_auto_;

        void autonomousCmd(double angle, double angle_inc);

        double dist_front_;
        double dist_right_;
        double dist_left_;

        double x_odom_, y_odom_;
        double yaw_;

        //std::vector<double> point_;
        //std::vector<std::vector<double>> ranges_;

        double state_auto_;
        int ddebugg_laser_;
        int past_state_;

        std::vector<double> x_vect_, y_vect_;
        std::mutex mutex_;

        geometry_msgs::msg::Twist msg_;



};

