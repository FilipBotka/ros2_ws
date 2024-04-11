#include "../include/process.h"


Process::Process()
    : Node("cmd_process")
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");

    subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_prcs", 10, std::bind(&Process::cmdCallback, this, std::placeholders::_1));


}

void Process::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    message_.linear.x = msg->linear.x;
    message_.angular.z = msg->angular.z;
    RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Process::getPublisher() const 
{
    return publisher_;
}

rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr Process::getSubscriber() const 
{
    return subscriber_;
}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::Node::SharedPtr node = std::make_shared<Process>();

    rclcpp::WallRate loop_rate(10);

    auto process_node = std::dynamic_pointer_cast<Process>(node);
    auto publisher = process_node->getPublisher();
    auto subscriber = process_node->getSubscriber();

    while (rclcpp::ok()) {
        publisher->publish(process_node->message_);

        rclcpp::spin_some(process_node->get_node_base_interface());

        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}