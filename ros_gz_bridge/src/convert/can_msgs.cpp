#include "ros_gz_bridge/convert/can_msgs.hpp"

namespace ros_gz_bridge
{

template<>
void convert_gz_to_ros(const gz::msgs::CanFrame& gz_msg, can_msgs::msg::Frame& ros_msg)
{
    ros_msg.header.stamp.sec = gz_msg.header().stamp().sec();
    ros_msg.header.stamp.nanosec = gz_msg.header().stamp().nsec();

    // Convert CAN frame fields
    ros_msg.id = gz_msg.id();
    ros_msg.is_rtr = gz_msg.is_rtr();
    ros_msg.is_extended = gz_msg.is_extended();
    ros_msg.is_error = gz_msg.is_error();
    ros_msg.dlc = static_cast<uint8_t>(gz_msg.dlc());

    // Convert data bytes
    const std::string& data = gz_msg.data();
    for (size_t i = 0; i < 8 && i < data.size(); ++i) {
        ros_msg.data[i] = static_cast<uint8_t>(data[i]);
    }
    // Zero-fill remaining bytes if data is shorter than 8 bytes
    for (size_t i = data.size(); i < 8; ++i) {
        ros_msg.data[i] = 0;
    }
}

template<>
void convert_ros_to_gz(const can_msgs::msg::Frame& ros_msg, gz::msgs::CanFrame& gz_msg)
{
    // Convert CAN frame fields
    gz_msg.set_id(ros_msg.id);
    gz_msg.set_is_rtr(ros_msg.is_rtr);
    gz_msg.set_is_extended(ros_msg.is_extended);
    gz_msg.set_is_error(ros_msg.is_error);
    gz_msg.set_dlc(ros_msg.dlc);

    // Convert data bytes
    std::string data(reinterpret_cast<const char*>(ros_msg.data.data()), 8);
    gz_msg.set_data(data);
}
}
