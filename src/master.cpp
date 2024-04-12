#include "../include/master.h"


Master::Master()
    : Node("Master")
{
    publisher_ = this->create_publisher<std_msgs::msg::Int16>("/cmd_master", 10);
    RCLCPP_INFO(this->get_logger(), "Keyboard publisher has been started.");
}

void Master::getInput()
{
    
    int16_t input;
    std::string line;
    while (rclcpp::ok())
    {
        std::cout << "Enter an integer: ";
        std::getline(std::cin, line);
        std::stringstream(line) >> input;

        if (!line.empty() && line.find_first_not_of("0123456789") == std::string::npos)
        {
            RCLCPP_INFO(this->get_logger(), "Received integer: %d", input);
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Invalid input. Please enter a valid integer.");
        }

        RCLCPP_INFO(this->get_logger(), "Integer to publisher: %d", input);

        std_msgs::msg::Int16 msg;
        msg.data = input;

        publisher_->publish(msg);
    }
    
}

rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr Master::getPublisher() const
{
    return publisher_;
}


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<Master>();
    auto publisher = node->getPublisher();

    while (rclcpp::ok())
    {
        node->getInput();      
        
    }
    

    

    rclcpp::shutdown();
    return 0;
}