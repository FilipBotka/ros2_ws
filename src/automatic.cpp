#include "../include/automatic.h"

Automatic::Automatic()
    : Node("Automatic"), distance_(0.5)
{
    plt::figure();
    subscriber_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
    "/scan", 10, std::bind(&Automatic::laserScanCallback, this, std::placeholders::_1));

    sub_master_ = this->create_subscription<std_msgs::msg::Int16>(
    "/cmd_master", 10, std::bind(&Automatic::masterCallback, this, std::placeholders::_1));

    sub_vel_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_vel", 10, std::bind(&Automatic::cmdCallback, this, std::placeholders::_1));

    sub_vel_ = this->create_subscription<geometry_msgs::msg::Twist>(
    "/cmd_auto", 10, std::bind(&Automatic::cmdPrcsCallback, this, std::placeholders::_1));

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");

    timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), 
            std::bind(&Automatic::timerCallback, this));

    timer_plot_ = this->create_wall_timer(
            std::chrono::milliseconds(500), 
            std::bind(&Automatic::timerPlotCallback, this));


}
void Automatic::timerPlotCallback()
{
    //std::lock_guard<std::mutex> lock(mutex_); // Protect data access
    plt::clf(); // Clear the current figure
    plt::plot(x_vect_, y_vect_, "b");
    plt::draw(); 
    plt::pause(0.01);
}

void Automatic::timerCallback()
{
    if (state_ == 2) 
    {
        geometry_msgs::msg::Twist msg;
        msg.angular.z = vel_angular_;
        msg.linear.x = vel_linear_;
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "Automatic state");
        //RCLCPP_INFO(this->get_logger(), "vel_linear Timer: %f", vel_linear_);
        //RCLCPP_INFO(this->get_logger(), "vel_angular Timer: %f", vel_angular_);
    }

}

void Automatic::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    //vel_linear_ = msg->linear.x;
    //vel_angular_ = msg->angular.z;
    //RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

void Automatic::cmdPrcsCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{   
    // tu by sa mohol spravit If ze ked state je 2 tak sa nebudu subscribovat nove spravi
    if(state_ == 1 && (msg->linear.x != 0 || msg->angular.z != 0)) 
    {
        vel_linear_ = msg->linear.x;
        vel_angular_ = msg->angular.z;
        //RCLCPP_INFO(this->get_logger(), "vel_linear: %f", vel_linear_);
        //RCLCPP_INFO(this->get_logger(), "vel_angular: %f", vel_angular_); 
    }

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
    //sleep(1);

    x_vect_.clear();
    y_vect_.clear();

    double angle_min = msg->angle_min;
    double angle_max = msg->angle_max;
    double angle_inc = msg->angle_increment;
    int range_size = msg->ranges.size();
    double range;

    dist_front_ = msg->ranges[0];
    RCLCPP_INFO(this->get_logger(), "dist_front: %f", dist_front_);

/*     if(dist_front_ < 0.55)
    {
        vel_angular_ = 0;
        vel_linear_ = 0;
        msg_.linear.x = 0;
        msg_.angular.z = 0;
        publisher_->publish(msg_);

    } */

    double x,y;
    //std::vector<double> x_vect, y_vect;
    std::vector<double> point;
    std::vector<std::vector<double>> points;
    double angle = angle_min;


    for (int i = 0; i < range_size; i++)
    {
        
        if(angle<=1.5707 || angle>=4.712) // chcem brat iba 180 pred sebou
        {
            range = msg->ranges[i];
            if(range<msg->range_max && range>msg->range_min)
            {
                x = range * cos(angle);
                y = range * sin(angle);
                point = {x,y};
                points.push_back(point);
                //plt::plot(x,y);

                x_vect_.push_back(x);
                y_vect_.push_back(y);

            }


        }
        //x.push_back(range * cos(angle))
        //y.push_back(range * sin(angle))
        
        angle += angle_inc;
        
    }

    /* for(double dist : x_vect)
    {
        if(dist < 0.55)
        {
            vel_angular_ = 0;
            vel_linear_ = 0;
            msg_.linear.x = 0;
            msg_.angular.z = 0;
            publisher_->publish(msg_);
        }
    } */


    double xp = x_vect_[0];
    double yp = y_vect_[0];

    RCLCPP_INFO(this->get_logger(), "x: %f", xp);
    RCLCPP_INFO(this->get_logger(), "y: %f", yp);

    //plt::figure();
    //plt::plot(x_vect_, y_vect_, "b");
    //plt::plot(x,y, "r");
    //plt::show();
     

}

void Automatic::autonomousCmd()
{

    

    if(dist_front_ < 0.55)
    {
        msg_.linear.x = 0;
        msg_.angular.z = 0;
        publisher_->publish(msg_);

    }

    
}

rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr Automatic::getPublisher() const 
{
    return publisher_;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);

    //std::shared_ptr<Automatic> node = std::make_shared<Automatic>();
    //rclcpp::Node::SharedPtr node = std::make_shared<Automatic>();
    //auto process_node = std::dynamic_pointer_cast<Automatic>(node);
    auto node = std::make_shared<Automatic>();
    // Timer, ktory bude spinovany 

    /* rclcpp::WallRate loop_rate(10);
    geometry_msgs::msg::Twist msg;
    auto publisher = process_node->getPublisher(); */

    rclcpp::spin(node);

    /* while(rclcpp::ok())
    {
        RCLCPP_INFO(process_node->get_logger(), "While loop");
        //rclcpp::spin_some(process_node);
        rclcpp::spin_some(process_node->get_node_base_interface());

        if(process_node->state_ == 2)
        {
            msg.angular.z = process_node->vel_angular_;
            msg.linear.x = process_node->vel_linear_;
            publisher->publish(msg);
            RCLCPP_INFO(process_node->get_logger(), "Automatic state");
        }

        loop_rate.sleep();
    } */


    rclcpp::shutdown();
    return 0;
}