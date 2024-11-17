#include "krever_base/krever_hardware.hpp"
#include <wiringPi.h>
#include "krever_base/motor_encoder.h"
#include "krever_base/MotorDriver.h"
namespace krever_base
{
kreverHardware::kreverHardware()
: logger_(rclcpp::get_logger("kreverHardware"))
{}
CallbackReturn kreverHardware::on_init(const hardware_interface::HardwareInfo & info)
{
if (hardware_interface::SystemInterface::on_init(info) != CallbackReturn::SUCCESS)
{
return CallbackReturn::ERROR;
}
RCLCPP_INFO(logger_, "Initializing...");
// Parse parameters directly from info.hardware_parameters (std::map)
try {
config_.left_wheel_name = info.hardware_parameters.at("left_wheel_name");
config_.right_wheel_name = info.hardware_parameters.at("right_wheel_name");
config_.enc_ticks_per_rev =
std::stoi(info.hardware_parameters.at("enc_ticks_per_rev"));
config_.loop_rate = std::stod(info.hardware_parameters.at("loop_rate"));
} catch (const std::out_of_range &e) {
RCLCPP_ERROR(logger_, "Missing required parameters in hardware configuration: %s",
e.what());
return CallbackReturn::ERROR;
} catch (const std::invalid_argument &e) {
RCLCPP_ERROR(logger_, "Invalid parameter value in hardware configuration: %s",
e.what());
return CallbackReturn::ERROR;
}
// Set up wheels
left_wheel_.setup(config_.left_wheel_name, config_.enc_ticks_per_rev);
right_wheel_.setup(config_.right_wheel_name, config_.enc_ticks_per_rev);
RCLCPP_INFO(logger_, "Finished initialization");
return CallbackReturn::SUCCESS;
}
std::vector<hardware_interface::StateInterface> kreverHardware::export_state_interfaces()

{
std::vector<hardware_interface::StateInterface> state_interfaces;
state_interfaces.emplace_back(hardware_interface::StateInterface(
left_wheel_.name, hardware_interface::HW_IF_VELOCITY, &left_wheel_.velocity));
state_interfaces.emplace_back(hardware_interface::StateInterface(
left_wheel_.name, hardware_interface::HW_IF_POSITION, &left_wheel_.position));
state_interfaces.emplace_back(hardware_interface::StateInterface(
right_wheel_.name, hardware_interface::HW_IF_VELOCITY, &right_wheel_.velocity));
state_interfaces.emplace_back(hardware_interface::StateInterface(
right_wheel_.name, hardware_interface::HW_IF_POSITION, &right_wheel_.position));
return state_interfaces;
}
std::vector<hardware_interface::CommandInterface>
kreverHardware::export_command_interfaces()
{
std::vector<hardware_interface::CommandInterface> command_interfaces;
command_interfaces.emplace_back(hardware_interface::CommandInterface(
left_wheel_.name, hardware_interface::HW_IF_VELOCITY, &left_wheel_.command));
command_interfaces.emplace_back(hardware_interface::CommandInterface(
right_wheel_.name, hardware_interface::HW_IF_VELOCITY,
&right_wheel_.command));
return command_interfaces;
}
CallbackReturn kreverHardware::on_configure(const rclcpp_lifecycle::State &
/*previous_state*/)
{
RCLCPP_INFO(logger_, "Configuring motors and encoders...");
// Initialize motor driver
Motor_Init();
// Initialize wiringPi using GPIO BCM pin numbers
wiringPiSetupGpio();
// Setup GPIO encoder interrupt and direction pins
pinMode(LEFT_WHL_ENC_INT, INPUT);
pinMode(RIGHT_WHL_ENC_INT, INPUT);
pinMode(LEFT_WHL_ENC_DIR, INPUT);
pinMode(RIGHT_WHL_ENC_DIR, INPUT);
// Setup pull-up resistors on encoder interrupt pins

pullUpDnControl(LEFT_WHL_ENC_INT, PUD_UP);
pullUpDnControl(RIGHT_WHL_ENC_INT, PUD_UP);
// Initialize encoder interrupts for falling signal states
wiringPiISR(LEFT_WHL_ENC_INT, INT_EDGE_FALLING, left_wheel_pulse);
wiringPiISR(RIGHT_WHL_ENC_INT, INT_EDGE_FALLING, right_wheel_pulse);
RCLCPP_INFO(logger_, "Successfully configured motors and encoders!");
return CallbackReturn::SUCCESS;
}
CallbackReturn kreverHardware::on_activate(const rclcpp_lifecycle::State &
/*previous_state*/)
{
RCLCPP_INFO(logger_, "Starting controller...");
return CallbackReturn::SUCCESS;
}
CallbackReturn kreverHardware::on_deactivate(const rclcpp_lifecycle::State &
/*previous_state*/)
{
RCLCPP_INFO(logger_, "Stopping Controller...");
return CallbackReturn::SUCCESS;
}
hardware_interface::return_type kreverHardware::read(const rclcpp::Time & /*time*/,
const rclcpp::Duration & period)
{
double delta_seconds = period.seconds();
// Read encoder values
read_encoder_values(&left_wheel_.encoder_ticks, &right_wheel_.encoder_ticks);
// Calculate wheel positions and velocities
double previous_position = left_wheel_.position;
left_wheel_.position = left_wheel_.calculate_encoder_angle();
left_wheel_.velocity = (left_wheel_.position - previous_position) / delta_seconds;
previous_position = right_wheel_.position;
right_wheel_.position = right_wheel_.calculate_encoder_angle();
right_wheel_.velocity = (right_wheel_.position - previous_position) / delta_seconds;
return hardware_interface::return_type::OK;
}

hardware_interface::return_type kreverHardware::write(const rclcpp::Time & /*time*/,
const rclcpp::Duration & period)
{
double left_motor_counts_per_loop = left_wheel_.command / left_wheel_.rads_per_tick
/ config_.loop_rate;
double right_motor_counts_per_loop = right_wheel_.command /
right_wheel_.rads_per_tick / config_.loop_rate;
// Send commands to motor driver
set_motor_speeds(left_motor_counts_per_loop, right_motor_counts_per_loop);
return hardware_interface::return_type::OK;
}
} // namespace krever_base
#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(
krever_base::kreverHardware,
hardware_interface::SystemInterface)