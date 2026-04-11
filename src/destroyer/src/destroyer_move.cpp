#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <std_msgs/msg/string.hpp>
#include <string>
#include <cmath>
#include <array>

using std::placeholders::_1;

class DestroyerMove : public rclcpp::Node
{
   public:
      DestroyerMove() : Node("move_destroyer") {
         power_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>("/power_destroyer", 10, std::bind(&DestroyerMove::powerLogger, this, _1));
         type_subscription_ = this->create_subscription<std_msgs::msg::String>("/move_type", 10, std::bind(&DestroyerMove::typeLogger, this, _1));
      }

   private:
      struct OmniWheelCommand {
         double fl;
         double fr;
         double bl;
         double br;
      };

      static std::string motionTranslator(const geometry_msgs::msg::Twist &message) {
         std::string msg;
         if (message.linear.x != 0.0) message.linear.x > 0.0 ? msg += "Maju " : msg += "Mundur ";
         if (message.linear.y != 0.0) message.linear.y > 0.0 ? msg += "Geser Kiri " : msg += "Geser Kanan ";
         if (message.angular.z != 0.0) message.angular.z > 0.0 ? msg += "Berputar Kiri" : msg += "Berputar Kanan";
         if (message.linear.x == 0.0 && message.linear.y == 0.0 && message.angular.z == 0.0) msg = "Diam";
         return msg;
      }

      static OmniWheelCommand omniCalc(const geometry_msgs::msg::Twist &message) {
         const double vx = message.linear.x;
         const double vy = message.linear.y;
         const double wz = message.angular.z;

        return OmniWheelCommand {
            vx + vy + wz,  // FL
            vx - vy - wz,  // FR
            vx - vy + wz,  // BL
            vx + vy - wz   // BR
        };
      }

      static std::string wheelDirection(double value) {
         if (value == 0.0) return "Diam";
         return value > 0.0 ? "Maju" : "Mundur";
      }

      void powerLogger(const geometry_msgs::msg::Twist::SharedPtr message) {
         const OmniWheelCommand wheel_command = omniCalc(*message);

         RCLCPP_INFO(this->get_logger(), "Orientasi Gerak: %s", motionTranslator(*message).c_str());
         RCLCPP_INFO(
            this->get_logger(),
            "Kombinasi Omni Wheels: FL=%s (%.2f), FR=%s (%.2f), BL=%s (%.2f), BR=%s (%.2f)",
            wheelDirection(wheel_command.fl).c_str(), wheel_command.fl,
            wheelDirection(wheel_command.fr).c_str(), wheel_command.fr,
            wheelDirection(wheel_command.bl).c_str(), wheel_command.bl,
            wheelDirection(wheel_command.br).c_str(), wheel_command.br);
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