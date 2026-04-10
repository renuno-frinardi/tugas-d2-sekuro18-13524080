#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>
#include <string>
#include <cmath>

using std::placeholders::_1;

class DestroyerMove : public rclcpp::Node
{
   public:
      DestroyerMove() : Node("move_destroyer") {
         power_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("/power_destroyer", 10, std::bind(&DestroyerMove::powerLogger, this, _1));
         type_subscription_ = this->create_subscription<std_msgs::msg::String>("/move_type", 10, std::bind(&DestroyerMove::typeLogger, this, _1));
      }

   private:
      static std::string motionTranslator(const geometry_msgs::msg::Twist &message) {
         std::string msg;
         message.linear.x > 0.0 ? msg + "Maju" : msg + "Mundur";
         message.linear.y > 0.0 ? msg + "Geser Kiri" : msg + "Geser Kanan";
         message.angular.z > 0.0 ? msg + "Berputar Kiri" : msg + "Berputar Kanan";
         return msg;
      }

      void powerLogger(const geometry_msgs::msg::Twist::SharedPtr message) {
         RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: %s", motionTranslator(*message).c_str());
      }

      void typeLogger(const std_msgs::msg::String::SharedPtr message) {
         RCLCPP_INFO(this->get_logger(), "Tipe Kendali Aktif: %s", message->data.c_str());
      }

      rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr power_subscription_;
      rclcpp::Subscription<std_msgs::msg::String>::SharedPtr type_subscription_;
};

int main(int argc, char * argv[])
{
   rclcpp::init(argc, argv);
   auto node = std::make_shared<DestroyerMove>();
   rclcpp::spin(node);
   rclcpp::shutdown();
   return 0;
}