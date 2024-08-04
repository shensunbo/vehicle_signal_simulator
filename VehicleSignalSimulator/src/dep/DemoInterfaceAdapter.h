#pragma once

#include "InterfaceAdapter.h"

#include <cstdint>

class DemoInterfaceAdapter :public InterfaceAdapter {
public:
    ~DemoInterfaceAdapter() override {};

//HMI cmd
    void RequestActiveGuideline(const uint8_t& data) const override;
    void RequestEnableTransparentCarModel(const uint8_t& data) const override;

//vehicle data
    void SetSteeringWheelAngle(const int32_t& data) const override;
    void SetEachWheelPulseSignal(const uint8_t& wheel, const uint8_t& pulse) const override;
    void SetGear(const uint8_t& data) const override;
};
