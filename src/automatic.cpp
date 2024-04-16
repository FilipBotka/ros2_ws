#include "../include/automatic.h"

Automatic::Automatic()
    : Node("Automatic")
{
    plt::figure();
    subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
    "/scan", 10, std::bind(&Automatic::laserScanCallback, this, std::placeholders::_1));

    sub_master_ = this->create_subscription<std_msgs::msg::Int16>(
    "/cmd_master", 10, std::bind(&Automatic::masterCallback, this, std::placeholders::_1));

    sub_vel_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10, std::bind(&Automatic::cmdCallback, this, std::placeholders::_1));

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");


}

void Automatic::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    vel_linear_ = msg->linear.x;
    vel_angular_ = msg->angular.z;
    //RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

void Automatic::masterCallback(const std_msgs::msg::Int16::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "callback active");
    state_ = msg->data;
    RCLCPP_INFO(this->get_logger(), "state: %d", state_);
}


void Automatic::laserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    //sensor_msgs::msg::LaserScan msg;
    sleep(1);

    double angle_min = msg->angle_min;
    double angle_max = msg->angle_max;
    double angle_inc = msg->angle_increment;
    int range_size = msg->ranges.size();
    double range;

    double x,y;
    std::vector<double> x_vect, y_vect;
    std::vector<double> point;
    std::vector<std::vector<double>> points;
    double angle = angle_min;


    for (int i = 0; i < range_size; i++)
    {
        
        if(angle>=angle_max)
        {
            range = msg->ranges[i];
            if(range<msg->range_max && range>msg->range_min)
            {
                x = range * cos(angle);
                y = range * sin(angle);
                point = {x,y};
                points.push_back(point);
                //plt::plot(x,y);

                x_vect.push_back(x);
                y_vect.push_back(y);

            }


        }
        //x.push_back(range * cos(angle))
        //y.push_back(range * sin(angle))
        
        angle += angle_inc;
        
    }

    //plt::figure();
    plt::plot(x_vect, y_vect);
    plt::show();
     

}

rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Automatic::getPublisher() const 
{
    return publisher_;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    rclcpp::Node::SharedPtr node = std::make_shared<Automatic>();
    auto process_node = std::dynamic_pointer_cast<Automatic>(node);

    rclcpp::WallRate loop_rate(10);

    geometry_msgs::msg::Twist msg;
    auto publisher = process_node->getPublisher();

    while(rclcpp::ok())
    {
        rclcpp::spin_some(process_node->get_node_base_interface());
        if(process_node->state_ == 2)
        {
            msg.angular.z = process_node->vel_angular_;
            msg.linear.x = process_node->vel_linear_;
            publisher->publish(msg);
            RCLCPP_INFO(process_node->get_logger(), "Automatic state");
        }

        loop_rate.sleep();
    }


    rclcpp::shutdown();
    return 0;
}