#include <cstdio>
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/u_int8_multi_array.hpp>
#include <sensor_msgs/msg/joy.hpp>


class JoyControl : public rclcpp::Node
{
public:
  JoyControl()
  : Node("joy_control")
  {
    publisher_ = this->create_publisher<std_msgs::msg::UInt8MultiArray>("/motor_steps", 10);

    out_msg.data.resize(2); // Resize to hold two uint8_t values
    out_msg.data[0] = 1; // Initialize first element
    out_msg.data[1] = 1; // Initialize second element
    
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "/joy", 10,
      [this](const sensor_msgs::msg::Joy::SharedPtr msg) {
        // A button is usually button 0 on most controllers
        if (msg->buttons.size() > 0) {
          if (msg->buttons[0] == 1)
            publisher_->publish(out_msg);
          else if (msg->buttons[5] == 1) {
            current_motor = std::min(6, current_motor + 1);
            RCLCPP_INFO(this->get_logger(), "Changed current motor to: %d", current_motor);
            out_msg.data[0] = current_motor; 
          }
          else if (msg->buttons[4] == 1) {
            current_motor = std::max(1, current_motor - 1);
            RCLCPP_INFO(this->get_logger(), "Changed current motor to: %d", current_motor);
            out_msg.data[0] = current_motor;
          }
        }
      });

    RCLCPP_INFO(this->get_logger(), "JoyControl node has been created.");
  }

  ~JoyControl()
  {
    RCLCPP_INFO(this->get_logger(), "JoyControl node is being destroyed.");
  }

private:
  rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  std_msgs::msg::UInt8MultiArray out_msg;
  uint8_t current_motor = 0;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<JoyControl>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
