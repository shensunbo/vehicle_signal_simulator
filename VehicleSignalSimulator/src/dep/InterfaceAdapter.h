#pragma once

#include <cstdint>

class InterfaceAdapter {
public:
    virtual ~InterfaceAdapter() = default;

//HMI cmd
    virtual void SVM_HMI_Request_Active_Guideline_Process(const uint8_t& data) const = 0;
    virtual void SVM_HMI_Request_Enable_Transparent_Car_Model_Process(const uint8_t& data) const = 0;

//vehicle data
    virtual void SteeringWheelAngleProcess(const int32_t& data) const = 0;
    virtual void EachWheelPulseSignalProcess(const uint8_t& wheel, const uint8_t& pulse) const = 0;
    virtual void GearProcess(const uint8_t& data) const = 0;
};
