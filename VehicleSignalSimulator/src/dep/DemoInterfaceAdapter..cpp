#include "DemoInterfaceAdapter.h"
#include <iostream>

void DemoInterfaceAdapter::SVM_HMI_Request_Active_Guideline_Process(const uint8_t& data) const {
    std::cout << "SVM_HMI_Request_Active_Guideline_Process" << " value = " << static_cast<uint16_t>(data) << std::endl;
}
void DemoInterfaceAdapter::SVM_HMI_Request_Enable_Transparent_Car_Model_Process(const uint8_t& data) const {
    std::cout << "SVM_HMI_Request_Enable_Transparent_Car_Model_Process" << " value = " << static_cast<uint16_t>(data) << std::endl;
}
void DemoInterfaceAdapter::SteeringWheelAngleProcess(const int32_t& data) const {
    std::cout << "SteeringWheelAngleProcess" << " value = " << data << std::endl;
}
void DemoInterfaceAdapter::EachWheelPulseSignalProcess(const uint8_t& wheel, const uint8_t& pulse) const {
    std::cout << "EachWheelPulseSignalProcess" << " wheel = " << static_cast<uint16_t>(wheel) << " pulse = " << static_cast<uint16_t>(pulse) << std::endl;
}
void DemoInterfaceAdapter::GearProcess(const uint8_t& data) const {
    std::cout << "GearProcess" << " value = " << static_cast<uint16_t>(data) << std::endl;
}