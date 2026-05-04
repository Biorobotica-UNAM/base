#include <rclcpp/rclcpp.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <tf2/utils.h>

using namespace std::chrono_literals;

class VirtualJointPublisherNode : public rclcpp::Node
{
public:
    VirtualJointPublisherNode()
    : Node("virtual_joint_publisher_node")
    {
        RCLCPP_INFO(this->get_logger(), "Initializing virtual_joint_publisher_node...");

        joint_name_ = this->declare_parameter<std::string>("joint_name", "virtual_joint");
        odom_topic_ = this->declare_parameter<std::string>("odom_topic", "odom");
        output_topic_ = this->declare_parameter<std::string>("output_topic", "joint_states");

        sub_joint_x_     = joint_name_ + "/x";
        sub_joint_y_     = joint_name_ + "/y";
        sub_joint_theta_ = joint_name_ + "/theta";

        pub_ = this->create_publisher<sensor_msgs::msg::JointState>(output_topic_, 10);

        sub_ = this->create_subscription<nav_msgs::msg::Odometry>(
            odom_topic_,
            10,
            std::bind(&VirtualJointPublisherNode::odom_callback, this,
                      std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(),
            "virtual_joint_publisher_node ready — listening on '%s', publishing to '%s'",
            odom_topic_.c_str(), output_topic_.c_str());
    }

private:
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr sub_;
    rclcpp::Publisher<sensor_msgs::msg::JointState>::SharedPtr pub_;

    std::string joint_name_;
    std::string odom_topic_;
    std::string output_topic_;

    std::string sub_joint_x_;
    std::string sub_joint_y_;
    std::string sub_joint_theta_;

    void odom_callback(const nav_msgs::msg::Odometry::SharedPtr msg)
    {
        const double x = msg->pose.pose.position.x;
        const double y = msg->pose.pose.position.y;

        tf2::Quaternion q(
            msg->pose.pose.orientation.x,
            msg->pose.pose.orientation.y,
            msg->pose.pose.orientation.z,
            msg->pose.pose.orientation.w);

        const double theta = tf2::getYaw(q);

        sensor_msgs::msg::JointState js;
        js.header.stamp = msg->header.stamp;
        js.header.frame_id = "";

        js.name     = {sub_joint_x_, sub_joint_y_, sub_joint_theta_};
        js.position = {x, y, theta};

        pub_->publish(js);
    }
};

int main(int argc, char ** argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<VirtualJointPublisherNode>());
    rclcpp::shutdown();
    return 0;
}
