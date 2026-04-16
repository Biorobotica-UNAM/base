#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/bool.hpp"
#include "base_interfaces/msg/digital_readings.hpp"

class DigitalStopNode : public rclcpp::Node
{
public:
    DigitalStopNode() : Node("digital_stop_node"), prev_value_(false)
    {
        subscription_ = this->create_subscription<base_interfaces::msg::DigitalReadings>(
            "/hardware/base/digital_readings",
            10,
            std::bind(&DigitalStopNode::callback, this, std::placeholders::_1)
        );

        publisher_ = this->create_publisher<std_msgs::msg::Bool>("/navigate/stop", 10);
    }

private:
    void callback(const base_interfaces::msg::DigitalReadings::SharedPtr msg)
    {
        if (msg->values.size() <= 7)
        {
            RCLCPP_WARN(this->get_logger(), "No existe índice 7");
            return;
        }

        bool current_value = msg->values[7];

        if (!prev_value_ && current_value)
        {
            RCLCPP_INFO(this->get_logger(), "STOP activado");

            std_msgs::msg::Bool stop_msg;
            stop_msg.data = true;
            publisher_->publish(stop_msg);
        }

        prev_value_ = current_value;
    }

    rclcpp::Subscription<base_interfaces::msg::DigitalReadings>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr publisher_;
    bool prev_value_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<DigitalStopNode>());
    rclcpp::shutdown();
    return 0;
}