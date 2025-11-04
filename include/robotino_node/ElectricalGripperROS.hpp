#ifndef ELECTRICALGRIPPERROS_HPP_
#define ELECTRICALGRIPPERROS_HPP_

#include "rec/robotino/api2/ElectricalGripper.h"

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/msg/gripper_state.hpp"
#include "base_interfaces/srv/set_gripper_state.hpp"

class ElectricalGripperROS : public rec::robotino::api2::ElectricalGripper
{
public:
	ElectricalGripperROS(rclcpp::Node* node);
	~ElectricalGripperROS();

private:
	rclcpp::Node* node_;
	rclcpp::Publisher<base_interfaces::msg::GripperState>::SharedPtr gripper_pub_;
	rclcpp::Service<base_interfaces::srv::SetGripperState>::SharedPtr set_gripper_server_;
	base_interfaces::msg::GripperState gripper_msg_;

	void setGripperStateCallback(
		const std::shared_ptr<base_interfaces::srv::SetGripperState::Request> req,
		std::shared_ptr<base_interfaces::srv::SetGripperState::Response> res);

	void stateChangedEvent(int state);
};

#endif /* ELECTRICALGRIPPERROS_HPP_ */