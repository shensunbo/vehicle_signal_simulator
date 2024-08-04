#pragma once

#include <cstdint>

class InterfaceAdapter {
public:
    virtual ~InterfaceAdapter() = default;

//HMI cmd
    virtual void RequestActiveGuideline(const uint8_t& data) const = 0;
    virtual void RequestEnableTransparentCarModel(const uint8_t& data) const = 0;

//vehicle data
    virtual void SetSteeringWheelAngle(const int32_t& data) const = 0;
    virtual void SetEachWheelPulseSignal(const uint8_t& wheel, const uint8_t& pulse) const = 0;
    virtual void SetGear(const uint8_t& data) const = 0;
};
