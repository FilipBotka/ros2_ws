#include "../include/process.h"


Process::Process()
    : Node("cmd_process")
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");

    pub_auto_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_auto", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");

    subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_prcs", 10, std::bind(&Process::cmdCallback, this, std::placeholders::_1));

    sub_master_ = this->create_subscription<std_msgs::msg::Int16>(
    "/cmd_master", 10, std::bind(&Process::masterCallback, this, std::placeholders::_1));

}

void Process::masterCallback(const std_msgs::msg::Int16::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "callback active");
    state_ = msg->data;
    RCLCPP_INFO(this->get_logger(), "state: %d", state_);
}


void Process::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    message_.linear.x = msg->linear.x;
    message_.angular.z = msg->angular.z;
    //RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Process::getPublisher() const 
{
    return publisher_;
}

rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Process::getPubAuto() const 
{
    return pub_auto_;
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
    auto pub_auto = process_node->getPubAuto();

    while (rclcpp::ok()) {
        rclcpp::spin_some(process_node->get_node_base_interface());
        // Publishing keyboard cmd to kobuki robot
        if(process_node->state_==1)
        {
            publisher->publish(process_node->message_);
            pub_auto->publish(process_node->message_);
        }
       
        loop_rate.sleep();
    }

    rclcpp::shutdown();
    return 0;
}