#include "../include/automatic.h"

Automatic::Automatic()
    : Node("Automatic"), distance_(0.5), state_auto_(0), ddebugg_laser_(0)
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

    sub_odom_ = this->create_subscription<nav_msgs::msg::Odometry>(
    "/odom", 10, std::bind(&Automatic::odomCallback, this, std::placeholders::_1));

    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Input publisher started");

    timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), 
            std::bind(&Automatic::timerCallback, this));

    timer_plot_ = this->create_wall_timer(
            std::chrono::milliseconds(500), 
            std::bind(&Automatic::timerPlotCallback, this));
    
    timer_auto_ = this->create_wall_timer(
            std::chrono::milliseconds(100), 
            std::bind(&Automatic::timerAuto, this));


}
void Automatic::timerPlotCallback()
{
    /* std::lock_guard<std::mutex> lock(mutex_); 
    plt::clf(); // Clear the current figure
    plt::plot(x_vect_, y_vect_, "b");

    std::vector<double> x_odom_vec = {x_odom_};
    std::vector<double> y_odom_vec = {y_odom_};

    plt::scatter(x_odom_vec, y_odom_vec, 10.0);
    plt::draw(); 
    plt::pause(0.01); */
}

void Automatic::timerCallback()
{
    if (state_ == 2 && state_auto_ != 1) 
    {
        geometry_msgs::msg::Twist msg;
        msg.angular.z = vel_angular_;
        msg.linear.x = vel_linear_;
        publisher_->publish(msg);
        //RCLCPP_INFO(this->get_logger(), "Automatic state");
        //RCLCPP_INFO(this->get_logger(), "vel_linear Timer: %f", vel_linear_);
        //RCLCPP_INFO(this->get_logger(), "vel_angular Timer: %f", vel_angular_);
        
        
    }

}


void Automatic::timerAuto()
{
    if (state_auto_ == 1 && state_ != 1)
    {
        msg_.linear.x = 0;
        msg_.angular.z = 0.3;
        publisher_->publish(msg_);
        RCLCPP_INFO(this->get_logger(), "auto Timer ide");
        // while yaw tu treba dat
        /* while (yaw_<1.54)
        {
            publisher_->publish(msg_);
        }
        msg_.linear.x = 0;
        msg_.angular.z = 0;
        publisher_->publish(msg_); */


    }

    if (past_state_ != state_)
    {
        state_auto_ = 0;
    }
    
    
    past_state_ = state_;

}

void Automatic::odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
{
    x_odom_ = msg->pose.pose.position.x;
    y_odom_ = msg->pose.pose.position.y;
    double x = msg->pose.pose.orientation.x;
    double y = msg->pose.pose.orientation.y;
    double z = msg->pose.pose.orientation.z;
    double w = msg->pose.pose.orientation.w;

    yaw_ = atan2( 2*(w*z + x*y), 1-2*(pow(z,2) + pow(y,2)));
    //RCLCPP_INFO(this->get_logger(), "\n yaw: %f", yaw_);

    //RCLCPP_INFO(this->get_logger(), "\npossition x: '%f\n' possition y: '%f\n'", x,y);

}

void Automatic::cmdCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    //vel_linear_ = msg->linear.x;
    //vel_angular_ = msg->angular.z;
    //RCLCPP_INFO(this->get_logger(), "Invalid key.");

}

void Automatic::cmdPrcsCallback(const geometry_msgs::msg::Twist::SharedPtr msg)
{   

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
    

    std::lock_guard<std::mutex> lock(mutex_);
    
    //std::ofstream outputFile("../ros2_ws/LaserDebugg.csv", std::ios::app);
    //std::ofstream outputFile2("../ros2_ws/LaserDebuggAll.csv", std::ios::app);

    x_vect_.clear();
    y_vect_.clear();

    double angle_min = msg->angle_min;
    double angle_max = msg->angle_max;
    double angle_inc = msg->angle_increment;
    int range_size = msg->ranges.size();
    double range;

    dist_front_ = msg->ranges[0];
    //RCLCPP_INFO(this->get_logger(), "dist_front: %f", dist_front_);

    std::vector<std::vector<double>> ranges_;
    std::vector<double> point_;

    double x,y;
    //std::vector<double> x_vect, y_vect;
    
    double angle = angle_min;
    double angle_start = angle_min;


    for (int i = 0; i < range_size; i++)
    {
        
        if(angle<=1.5707 || angle>=4.712) // chcem brat iba 180 pred sebou
        {
            range = msg->ranges[i];
            if(range < 5.1 && range > 0.15)
            {
                // suradnice jednotlivych bodov
                x = range * cos(angle);
                y = range * sin(angle);
                //point = {x,y};
                //points.push_back(point);
                //plt::plot(x,y);

                x_vect_.push_back(x);
                y_vect_.push_back(y);
                point_.push_back(range);
                point_.push_back(angle);
                ranges_.push_back(point_);

                if (ddebugg_laser_ < 1)
                {
                    //outputFile2 << x << "," << range << "," << angle << "\n";
                }
                


            }


        }
        //x.push_back(range * cos(angle))
        //y.push_back(range * sin(angle))
        
        angle += angle_inc;
        
    }

    RCLCPP_INFO(this->get_logger(), "range size: %d", ranges_.size());

    // Toto by malo cisto brat iba vzdialenost na danom uhle
    if (state_auto_ == 0)
    {
        for(std::vector<double> rg : ranges_)
        {
            RCLCPP_INFO(this->get_logger(), "range: %f", rg[0]);
            if(rg[0] < 0.5)
            {
                vel_angular_ = 0;
                vel_linear_ = 0;
                msg_.linear.x = 0;
                msg_.angular.z = 0;
                state_auto_ = 1;
                double min_range_angle = rg[1];
                publisher_->publish(msg_);
                RCLCPP_INFO(this->get_logger(), "laser scan distance %f", rg[0]);

                
            }
        }
    }
    


    /* if(state_auto_ == 0)
    {
        int iter = 0;
        //autonomousCmd(angle_start, angle_inc);
        for(double dist : x_vect_)
        {
            if(dist < 0.2)
            {
                vel_angular_ = 0;
                vel_linear_ = 0;
                msg_.linear.x = 0;
                msg_.angular.z = 0;
                publisher_->publish(msg_);
                RCLCPP_INFO(this->get_logger(), "reached dist %f on angel %f", dist, y_vect_[iter]);
                RCLCPP_INFO(this->get_logger(), "laser scan x size %d", x_vect_.size());
                RCLCPP_INFO(this->get_logger(), "laser scan y size %d", y_vect_.size());

            }
            
            if(ddebugg_laser_ < 1)
            {
                outputFile << dist << "," << y_vect_[iter] << "\n"; 
            }

            iter++;
        } 

        ddebugg_laser_++;
    }*/


    /* double xp = x_vect_[0];
    double yp = y_vect_[0];

    RCLCPP_INFO(this->get_logger(), "x: %f", xp);
    RCLCPP_INFO(this->get_logger(), "y: %f", yp); */

}

void Automatic::autonomousCmd(double angle, double angle_inc)
{
    double min_dist = 10;
    double min_dist_angle = 0;
    for(double x : x_vect_)
    {
        if (x<min_dist)
        {
            min_dist = x;
            min_dist_angle = angle; 
        }
        
        angle += angle_inc;

    }

    double onPlane = 6.28-angle;

    double fullRot = onPlane + 1.57079633;

    /* while (dist_front_ <= 0.55)
    {
        msg_.angular.z = 0.1;
        msg_.linear.x = 0;
        publisher_->publish(msg_);
    }

    msg_.angular.z = 0;
    msg_.linear.x = 0;
    publisher_->publish(msg_); */



    
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