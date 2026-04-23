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

        publisher_ = this->create_publisher<std_msgs::msg::Bool>("/navigation/navigation_ctrl/stop", 10);
    }

private:
    void callback(const base_interfaces::msg::DigitalReadings::SharedPtr msg)
    {
        bool current_value = msg->values[6];

        std_msgs::msg::Bool stop_msg;
        stop_msg.data = current_value;

        publisher_->publish(stop_msg);

        if (current_value)
        {
            RCLCPP_INFO_THROTTLE(
                this->get_logger(),
                *this->get_clock(),
                2000,
                "STOP activo"
            );
        }
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