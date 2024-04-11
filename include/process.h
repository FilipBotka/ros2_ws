#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "geometry_msgs/msg/twist.hpp"

class Process : public rclcpp::Node
{
    public:
        Process();

        void cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg);
        
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr getPublisher() const;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr getSubscriber() const;

        geometry_msgs::msg::Twist message_;

    private:
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subscriber_;

};