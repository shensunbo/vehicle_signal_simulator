#pragma once

#include "InterfaceAdapter.h"

#include <cstdint>

class DemoInterfaceAdapter :public InterfaceAdapter {
public:
    ~DemoInterfaceAdapter() override {};

//HMI cmd
    void SVM_HMI_Request_Active_Guideline_Process(const uint8_t& data) const override;
    void SVM_HMI_Request_Enable_Transparent_Car_Model_Process(const uint8_t& data) const override;

//vehicle data
    void SteeringWheelAngleProcess(const int32_t& data) const override;
    void EachWheelPulseSignalProcess(const uint8_t& wheel, const uint8_t& pulse) const override;
    void GearProcess(const uint8_t& data) const override;
};
