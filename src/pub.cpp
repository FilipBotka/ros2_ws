#include "../include/uds_kobuki/pub.h"

Cmd::Cmd()
    : Node("control_cmd")
{
    publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
    RCLCPP_INFO(this->get_logger(), "Keyboard publisher has been started.");
}

void Cmd::publishCommand(char key)
{
    auto message = geometry_msgs::msg::Twist();

    switch (key) {
        case 'A': // Sipka hore
            message.linear.x = 0.5;
            break;
        case 'B': // Sipka dole
            message.linear.x = -0.5;
            break;
        case 'C': // Sipka vpravo
            message.angular.z = -1.0;
            break;
        case 'D': // Sipka vlavo
            message.angular.z = 1.0;
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "Invalid key.");
    }

    publisher_->publish(message);
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
    auto node = std::make_shared<Cmd>();

    std::cout << "Control the robot with arrow keys." << std::endl;

    while (rclcpp::ok()) {
        char c = node->getch();  // Call your non-blocking input function
        if (c == '\033') { // if the first value is esc
            node->getch();      // skip the [
            c = node->getch();  // get the actual value
            node->publishCommand(c);
        }
    }

    rclcpp::shutdown();
    return 0;
}
