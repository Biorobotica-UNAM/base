#ifndef POWERMANAGEMENTROS_HPP_
#define POWERMANAGEMENTROS_HPP_

#include "rec/robotino/api2/PowerManagement.h"

#include "rclcpp/rclcpp.hpp"
#include "base_interfaces/msg/power_readings.hpp"

class PowerManagementROS: public rec::robotino::api2::PowerManagement
{
public:
	PowerManagementROS(rclcpp::Node* node);
	~PowerManagementROS();

private:
	rclcpp::Node* node_;
	rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<base_interfaces::msg::PowerReadings>::SharedPtr publisher_;

	void readingsEvent(float current, float voltage);
	void powerTimerCb();
};
#endif /* POWERMANAGEMENTROS_HPP_ */