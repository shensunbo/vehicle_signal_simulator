#pragma once

#include "InterfaceAdapter.h"
#include "gmock/gmock.h"

class MockInterfaceAdapter : public InterfaceAdapter {
public:
    MOCK_METHOD(void, SVM_HMI_Request_Active_Guideline_Process, (const uint8_t& data), (const, override));
    MOCK_METHOD(void, SVM_HMI_Request_Enable_Transparent_Car_Model_Process, (const uint8_t& data), (const, override));
    MOCK_METHOD(void, SteeringWheelAngleProcess, (const int32_t& data), (const, override));
    MOCK_METHOD(void, EachWheelPulseSignalProcess, (const uint8_t& wheel, const uint8_t& pulse), (const, override));
    MOCK_METHOD(void, GearProcess, (const uint8_t& data), (const, override));
};