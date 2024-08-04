#pragma once

#include "InterfaceAdapter.h"
#include "gmock/gmock.h"

class MockInterfaceAdapter : public InterfaceAdapter {
public:
    MOCK_METHOD(void, RequestActiveGuideline, (const uint8_t& data), (const, override));
    MOCK_METHOD(void, RequestEnableTransparentCarModel, (const uint8_t& data), (const, override));
    MOCK_METHOD(void, SetSteeringWheelAngle, (const int32_t& data), (const, override));
    MOCK_METHOD(void, SetEachWheelPulseSignal, (const uint8_t& wheel, const uint8_t& pulse), (const, override));
    MOCK_METHOD(void, SetGear, (const uint8_t& data), (const, override));
};