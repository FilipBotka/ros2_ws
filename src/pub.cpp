#include "../include/pub.h"

Cmd::Cmd()
    : Node("control_cmd")
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_prcs", 10);
    RCLCPP_INFO(this->get_logger(), "Keyboard publisher has been started.");

    sub_master_ = this->create_subscription<std_msgs::msg::Int16>(
    "/cmd_master", 10, std::bind(&Cmd::masterCallback, this, std::placeholders::_1));
}

void Cmd::masterCallback(const std_msgs::msg::Int16::SharedPtr msg)
{
    RCLCPP_INFO(this->get_logger(), "callback active");
    state_ = msg->data;
    RCLCPP_INFO(this->get_logger(), "state: %d", state_);
}


void Cmd::publishCommand(char key)
{

    switch (key) {
        case 'A': // Sipka hore
            message_.linear.x += 0.1;
            break;
        case 'B': // Sipka dole
            message_.linear.x += -0.1;
            break;
        case 'C': // Sipka vpravo
            message_.angular.z += -0.1;
            break;
        case 'D': // Sipka vlavo
            message_.angular.z += 0.1;
            break; 
        case '\n':
            message_.linear.x = 0;
            message_.angular.z = 0;
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "Invalid key.");
    }

    publisher_->publish(message_);
}

char Cmd::getch()
{
    char buf = 0;
    struct termios old; // sem ukladam stare nastavenia
    struct termios new_; 
    fflush(stdout);
    if (tcgetattr(0, &old) < 0) // Ukladam si predosle nastavenia terminalu pred tym ako ich zmenim
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) // Nastavujem svoje nastavenia -->aby citalo iba jeden input a necakalo na enter
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0) // Nacitam z klavesnice
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) // vratim stare nastavenie
        perror("tcsetattr ~ICANON");
    return (buf); // vraciam nacitanu hodnotu

}

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::Node::SharedPtr node = std::make_shared<Cmd>();

    rclcpp::WallRate loop_rate(10);

    auto process_node = std::dynamic_pointer_cast<Cmd>(node);


    std::cout << "Control the robot with arrow keys." << std::endl;

    while (rclcpp::ok()) {
        rclcpp::spin_some(process_node->get_node_base_interface());
        if(process_node->state_==1)
        {
           char c = process_node->getch();  // Call your non-blocking input function
            if (c == '\033') { // if the first value is esc
                process_node->getch();      // skip the [
                c = process_node->getch();  // get the actual value
                //node->publishCommand(c);
            }
            process_node->publishCommand(c); 
        }

        
    }

    rclcpp::shutdown();
    return 0;
}
