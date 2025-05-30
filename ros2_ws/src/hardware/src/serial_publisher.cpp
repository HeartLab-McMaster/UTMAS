#include <cstdio>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <cstring>


class SerialPublisher : public rclcpp::Node
{
public:
  SerialPublisher()
  : Node("serial_publisher")
  {
    // Open serial port
    serial_fd_ = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_SYNC);
    if (serial_fd_ < 0) {
      RCLCPP_ERROR(this->get_logger(), "Failed to open serial port: %s", strerror(errno));
    } else {
      configure_serial_port(serial_fd_);
      RCLCPP_INFO(this->get_logger(), "Serial port opened and configured.");
    }

    // Create a subscription to the "motor_steps" topic
    subscription_ = this->create_subscription<std_msgs::msg::UInt8MultiArray>(
      "/motor_steps", 10,
      [this](const std_msgs::msg::UInt8MultiArray::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), "Received serial data: [%d, %d]",
                    msg->data[0], msg->data[1]);
        if (serial_fd_ >= 0) {
          //Message is 2 bytes

          char buffer[2];
          
          buffer[0] = static_cast<char>(msg->data[0]);
          buffer[1] = static_cast<char>(msg->data[1]);

          write(serial_fd_, buffer, 2);
        }
      });
    RCLCPP_INFO(this->get_logger(), "SerialPublisher node has been created.");
  }

  ~SerialPublisher()
  {
    if (serial_fd_ >= 0) {
      close(serial_fd_);
      RCLCPP_INFO(this->get_logger(), "Serial port closed.");
    }
    RCLCPP_INFO(this->get_logger(), "SerialPublisher node is being destroyed.");
  }

  void configure_serial_port(int fd) {
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
      RCLCPP_ERROR(this->get_logger(), "Error from tcgetattr: %s", strerror(errno));
      return;
    }
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);

    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
    tty.c_iflag &= ~IGNBRK;         // disable break processing
    tty.c_lflag = 0;                // no signaling chars, no echo, no canonical processing
    tty.c_oflag = 0;                // no remapping, no delays
    tty.c_cc[VMIN]  = 0;            // read doesn't block
    tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

    tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls, enable reading
    tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
      RCLCPP_ERROR(this->get_logger(), "Error from tcsetattr: %s", strerror(errno));
    }
  }

  rclcpp::Subscription<std_msgs::msg::UInt8MultiArray>::SharedPtr subscription_;
  int serial_fd_{-1};
};


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<SerialPublisher>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
