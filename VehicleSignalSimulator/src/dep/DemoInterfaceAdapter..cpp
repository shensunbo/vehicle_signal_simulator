#include "DemoInterfaceAdapter.h"
#include <iostream>

void DemoInterfaceAdapter::RequestActiveGuideline(const uint8_t& data) const {
    std::cout << "RequestActiveGuideline" << " value = " << static_cast<uint16_t>(data) << std::endl;
}
void DemoInterfaceAdapter::RequestEnableTransparentCarModel(const uint8_t& data) const {
    std::cout << "RequestEnableTransparentCarModel" << " value = " << static_cast<uint16_t>(data) << std::endl;
}
void DemoInterfaceAdapter::SetSteeringWheelAngle(const int32_t& data) const {
    std::cout << "SetSteeringWheelAngle" << " value = " << data << std::endl;
}
void DemoInterfaceAdapter::SetEachWheelPulseSignal(const uint8_t& wheel, const uint8_t& pulse) const {
    std::cout << "SetEachWheelPulseSignal" << " wheel = " << static_cast<uint16_t>(wheel) << " pulse = " << static_cast<uint16_t>(pulse) << std::endl;
}
void DemoInterfaceAdapter::SetGear(const uint8_t& data) const {
    std::cout << "SetGear" << " value = " << static_cast<uint16_t>(data) << std::endl;
}