#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>

using std::placeholders::_1;
using namespace std::chrono_literals;

class DestroyerCheck : public rclcpp::Node
{
   public:
      DestroyerCheck() : Node("check_move_destroyer"), last_drive_msg_time_(this->now() - rclcpp::Duration::from_seconds(10.0)) {
         drive_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("/drive_destroyer", 10, std::bind(&DestroyerCheck::handleDrive, this, _1));
         autonomous_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("/autonomous_destroyer", 10, std::bind(&DestroyerCheck::handleAutonomous, this, _1));

         power_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/power_destroyer", 10);
         type_publisher_ = this->create_publisher<std_msgs::msg::String>("/move_type", 10);
      }

   private:
      void publishForwardMsg(const geometry_msgs::msg::Twist &message, const std::string &control_type) {
         power_publisher_->publish(message);

         std_msgs::msg::String type_message;
         type_message.data = control_type;
         type_publisher_->publish(type_message);

         RCLCPP_INFO(this->get_logger(), "Received from %s: linear.x=%.2f, linear.y=%.2f, yaw=%.2f", control_type.c_str(), message.linear.x, message.linear.y, message.angular.z);
      }

      void handleDrive(const geometry_msgs::msg::Twist::SharedPtr message) {
         last_drive_msg_time_ = this->now();
         publishForwardMsg(*message, "Driver");
      }

      void handleAutonomous(const geometry_msgs::msg::Twist::SharedPtr message) {
         const auto time_since_drive = this->now() - last_drive_msg_time_;
         if (time_since_drive < rclcpp::Duration::from_seconds(1.0)) return;
         publishForwardMsg(*message, "Autonomous");
      }

      rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr drive_subscription_;
      rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr autonomous_subscription_;
      rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr power_publisher_;
      rclcpp::Publisher<std_msgs::msg::String>::SharedPtr type_publisher_;
      rclcpp::Time last_drive_msg_time_;
};

int main(int argc, char * argv[])
{
   rclcpp::init(argc, argv);
   auto node = std::make_shared<DestroyerCheck>();
   rclcpp::spin(node);
   rclcpp::shutdown();
   return 0;
}