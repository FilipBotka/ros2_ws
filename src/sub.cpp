#include <functional>
#include "../include/uds_kobuki/sub.h"


Odometry::Odometry()
 : Node("odometry_sub")
{
    subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
    "/odom", 10, std::bind(&Odometry::odom_callback, this, std::placeholders::_1));
}

void Odometry::odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;

    RCLCPP_INFO(this->get_logger(), "\npossition x: '%f\n' possition y: '%f\n'", x,y);
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Odometry>());
  rclcpp::shutdown();
  return 0;
}

