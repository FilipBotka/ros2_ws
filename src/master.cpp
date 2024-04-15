#include "../include/master.h"


Master::Master()
    : Node("Master")
{
    publisher_ = this->create_publisher<std_msgs::msg::Int16>("/cmd_master", 10);
    RCLCPP_INFO(this->get_logger(), "Keyboard publisher has been started.");

    subscriber_ = this->create_subscription<nav_msgs::msg::Odometry>(
    "/odom", 10, std::bind(&Master::odom_callback, this, std::placeholders::_1));

    sub_vel_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10, std::bind(&Master::cmdCallback, this, std::placeholders::_1));
}

void Master::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    vel_linear_ = msg->linear.x;
    vel_angular_ = msg->angular.z;
    //RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

bool Master::isZero()
{
    if(vel_angular_ == 0 && vel_linear_ == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void Master::odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    double x = msg->pose.pose.position.x;
    double y = msg->pose.pose.position.y;

    RCLCPP_INFO(this->get_logger(), "\npossition x: '%f\n' possition y: '%f\n'", x,y);
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

        if (!line.empty() && line.find_first_not_of("123") == std::string::npos)
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

std_msgs::msg::Int16 Master::getInput2()
{
    int16_t input;
    std::string line;
    std::cout << "Enter an integer: ";
    std::getline(std::cin, line);
    std::stringstream(line) >> input;

    if (!line.empty() && line.find_first_not_of("0123") == std::string::npos)
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

    return msg;


}

rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr Master::getPublisher() const
{
    return publisher_;
}


int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::Node::SharedPtr node = std::make_shared<Master>();
    std_msgs::msg::Int16 msg_old;
    std_msgs::msg::Int16 msg;

    msg_old.data = 0;

    rclcpp::WallRate loop_rate(10);

    auto process_node = std::dynamic_pointer_cast<Master>(node);
    auto publisher = process_node->getPublisher();

    while (rclcpp::ok())
    {
        
        rclcpp::spin_some(process_node->get_node_base_interface());
        //publisher->publish(process_node->getInput2());   
        msg = process_node->getInput2();
        rclcpp::spin_some(process_node->get_node_base_interface());
        if(msg_old.data>0 && msg.data==0)
        {
            if (process_node->isZero()==true)
            {
                publisher->publish(msg);
                RCLCPP_INFO(process_node->get_logger(), "published stop cmd");
            }
            else
            {
                RCLCPP_ERROR(process_node->get_logger(), "Invalid state, velocities must be zero to Turn off the robot");
            }
            
        }
        else
        {
            publisher->publish(msg);
            RCLCPP_INFO(process_node->get_logger(), "published cmd");
            msg_old = msg;
        }
        

        loop_rate.sleep();
        
    }
    

    

    rclcpp::shutdown();
    return 0;
}