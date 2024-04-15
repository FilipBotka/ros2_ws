#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <iostream>
#include <termios.h>
#include <unistd.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int16.hpp"
#include "geometry_msgs/msg/pose_with_covariance.hpp"
#include "geometry_msgs/msg/twist.hpp"

class Cmd : public rclcpp::Node 
{
    public:
        Cmd();
        void publishCommand(char key);
        char getch();

        void masterCallback(const std_msgs::msg::Int16::SharedPtr msg);

        geometry_msgs::msg::Twist message_;
        int state_;

        

    private:
        rclcpp::TimerBase::SharedPtr timer_;

        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
        rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr sub_master_;

        size_t count_;


};