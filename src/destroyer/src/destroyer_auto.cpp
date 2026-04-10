#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <random>
#include <chrono>

#define MIN -10.0
#define MAX 10.0

using namespace std::chrono_literals;

class DestroyerAuto : public rclcpp::Node
{
   public:
      DestroyerAuto() : Node("auto_mode_destroyer"), generator_(std::random_device{}()), value_picker_(MIN, MAX) {
         publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/autonomous_destroyer", 10);
         timer_ = this->create_wall_timer(500ms, std::bind(&DestroyerAuto::publishRandMov, this));
         RCLCPP_INFO(this->get_logger(), "Autonomous mode activated. Publishing random movement commands every 500ms in range of %.2f to %.2f.", MIN, MAX);
      }

   private:
      void publishRandMov() {
         geometry_msgs::msg::Twist message;
         message.linear.x = value_picker_(generator_);
         message.linear.y = value_picker_(generator_);
         message.angular.z = value_picker_(generator_);

         publisher_->publish(message);
         RCLCPP_INFO(this->get_logger(), "Published autonomous command: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", message.linear.x, message.linear.y, message.angular.z);
      }

      rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
      rclcpp::TimerBase::SharedPtr timer_;
      std::mt19937 generator_;
      std::uniform_real_distribution<double> value_picker_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<DestroyerAuto>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}