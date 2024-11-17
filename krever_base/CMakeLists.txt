cmake_minimum_required(VERSION 3.8)
project(krever_base)

# Find required dependencies
find_package(ament_cmake REQUIRED)
find_package(hardware_interface REQUIRED)
find_package(pluginlib REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_lifecycle REQUIRED)
find_package(std_srvs REQUIRED)

# Create a shared library for the hardware interface
add_library(krever_hardware SHARED 
    src/krever_hardware.cpp 
    src/wheel.cpp 
    src/MotorDriver.c 
    src/motor_encoder.c
    src/DEV_Config.c
)

# Include directories
target_include_directories(krever_hardware PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies for krever_hardware
ament_target_dependencies(
    krever_hardware
    hardware_interface
    pluginlib
    rclcpp
    rclcpp_lifecycle
)

# Link WiringPi library to krever_hardware
target_link_libraries(krever_hardware wiringPi)

# Export hardware plugin
pluginlib_export_plugin_description_file(hardware_interface krever_hardware.xml)

# Install the hardware library and headers
install(
    DIRECTORY include/
    DESTINATION include/${PROJECT_NAME}
)

install(
    TARGETS krever_hardware
    DESTINATION lib
)

# Add motor_checks_server as an executable
add_executable(motor_checks_server 
    src/motor_checks_server.cpp 
    src/MotorDriver.c
    src/motor_encoder.c
)

target_link_libraries(motor_checks_server krever_hardware wiringPi)

# Include directories for motor_checks_server
target_include_directories(motor_checks_server PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies for motor_checks_server
ament_target_dependencies(motor_checks_server rclcpp std_srvs)
target_link_libraries(motor_checks_server wiringPi)

# Add motor_checks_client as an executable
add_executable(motor_checks_client src/motor_checks_client.cpp)

# Include directories for motor_checks_client
target_include_directories(motor_checks_client PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link dependencies for motor_checks_client
ament_target_dependencies(motor_checks_client rclcpp std_srvs)

# Install motor_checks_server and motor_checks_client
install(
    TARGETS 
        motor_checks_server 
        motor_checks_client
    DESTINATION lib/${PROJECT_NAME}
)

# Export library
ament_export_libraries(krever_hardware)

ament_package()