#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "MockInterfaceAdapter.h"
#include "MessagelProccessor.h"
#include "SvmSignalTypes.h"

#include <memory>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace testing;

//TODO: 连续运行单元测试会失败
#define WAIT_TIME (20)
//something about socket, sometimes unit will failed because socket is not close
class MessagelProccessorTest : public ::testing::Test {
public:
    void SetUp() override {
        const std::string CLIENT_IP = "127.0.0.1";
        const int PORT = 10000;

        clientFd_ = socket(AF_INET, SOCK_DGRAM, 0);
        ASSERT_NE(clientFd_, -1);

        clientAddr_.sin_family = AF_INET;
        clientAddr_.sin_port = htons(PORT);
        clientAddr_.sin_addr.s_addr = inet_addr(CLIENT_IP.c_str());

        mockInterfaceAdapter_ = std::make_shared<MockInterfaceAdapter>();
        svmSignalProcessor_ = std::make_shared<SvmSignalProcessor>(CLIENT_IP.c_str(), PORT, mockInterfaceAdapter_);

        auto svmSignalProcessorHandle = [this]() { return this->svmSignalProcessor_->serverRun(); };
        svmSignalProcessorThread_ =  new std::thread(svmSignalProcessorHandle);
    }

    void TearDown() override {
        SvmSignalData cmdData = {};
        cmdData.fid = 0x01;
        cmdData.cmd = SIMULATOR_EXIT;
        int ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
        ASSERT_NE(ret, -1);
        svmSignalProcessorThread_->join();
        delete svmSignalProcessorThread_;

        close(clientFd_);
    }
public:
    //server
    std::shared_ptr<MockInterfaceAdapter> mockInterfaceAdapter_;
    std::shared_ptr<SvmSignalProcessor> svmSignalProcessor_;
    std::thread *svmSignalProcessorThread_;

    //client
    int clientFd_;
    struct sockaddr_in clientAddr_;
};

TEST_F(MessagelProccessorTest, GuidelineTest) {
    int ret = 0;
    SvmSignalData cmdData = {};
    //TODO:add this delay can fix the ctest failed bug, :(
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

    EXPECT_CALL(*mockInterfaceAdapter_, RequestActiveGuideline(
        static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW)))
        .Times(1);
    cmdData.fid = 0x01;
    cmdData.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    //TODO: this is not good, but if not wait, sometimes failed
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    EXPECT_CALL(*mockInterfaceAdapter_, RequestActiveGuideline(
        static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW)))
        .Times(1);
    
    cmdData.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    EXPECT_CALL(*mockInterfaceAdapter_, RequestActiveGuideline(
        static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW)))
        .Times(1);
    
    cmdData.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    EXPECT_CALL(*mockInterfaceAdapter_, RequestActiveGuideline(
        static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW)))
        .Times(1);
    
    cmdData.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
}

TEST_F(MessagelProccessorTest, TransparentTest) {
    int ret = 0;
    SvmSignalData cmdData = {};
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    EXPECT_CALL(*mockInterfaceAdapter_, RequestEnableTransparentCarModel(
        static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::ENABLE_TRANSPARENT_CAR_MODEL)))
        .Times(1);

    cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::ENABLE_TRANSPARENT_CAR_MODEL);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    EXPECT_CALL(*mockInterfaceAdapter_, RequestEnableTransparentCarModel(
        static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::DISABLE_TRANSPARENT_CAR_MODEL)))
        .Times(1);

    cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::DISABLE_TRANSPARENT_CAR_MODEL);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
}

TEST_F(MessagelProccessorTest, SetSteeringWheelAngleTest) {
    int ret = 0;
    int8_t radians = 0;
    SvmSignalData cmdData = {};
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    radians = -9;
    EXPECT_CALL(*mockInterfaceAdapter_, SetSteeringWheelAngle(radians * 57))
        .Times(1);

    cmdData.fid = 0x01;
    cmdData.cmd = STEERING_WHEEL_ANGLE;
    memcpy(&cmdData.value, &radians, sizeof(cmdData.value));
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    radians = 9;
    EXPECT_CALL(*mockInterfaceAdapter_, SetSteeringWheelAngle(radians * 57))
        .Times(1);

    cmdData.fid = 0x01;
    cmdData.cmd = STEERING_WHEEL_ANGLE;
    memcpy(&cmdData.value, &radians, sizeof(cmdData.value));
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


    radians = 0;
    EXPECT_CALL(*mockInterfaceAdapter_, SetSteeringWheelAngle(radians * 57))
        .Times(1);

    cmdData.fid = 0x01;
    cmdData.cmd = STEERING_WHEEL_ANGLE;
    memcpy(&cmdData.value, &radians, sizeof(cmdData.value));
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
}

// TEST_F(MessagelProccessorTest, SetEachWheelPulseSignalTest) {
//     int ret = 0;
//     SvmSignalData cmdData = {};
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


//     EXPECT_CALL(*mockInterfaceAdapter_, SetEachWheelPulseSignal(_, _))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = 0;
//     cmdData.data[0] = 50;
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


//     EXPECT_CALL(*mockInterfaceAdapter_, SetEachWheelPulseSignal(_, _))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = 1;
//     cmdData.data[0] = 100;
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
// }

// TEST_F(MessagelProccessorTest, SetGearTest) {
//     int ret = 0;
//     SvmSignalData cmdData = {};
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));

//     EXPECT_CALL(*mockInterfaceAdapter_, SetGear(
//         static_cast<uint8_t>(Gear_value::P)))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = static_cast<uint8_t>(Gear_value::P);
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


//     EXPECT_CALL(*mockInterfaceAdapter_, SetGear(
//         static_cast<uint8_t>(Gear_value::D)))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = static_cast<uint8_t>(Gear_value::D);
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


//     EXPECT_CALL(*mockInterfaceAdapter_, SetGear(
//         static_cast<uint8_t>(Gear_value::R)))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = static_cast<uint8_t>(Gear_value::R);
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));


//     EXPECT_CALL(*mockInterfaceAdapter_, SetGear(
//         static_cast<uint8_t>(Gear_value::N)))
//         .Times(1);
    
//     cmdData.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;
//     cmdData.value = static_cast<uint8_t>(Gear_value::N);
//     ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
//     EXPECT_NE(ret, -1);
//     std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
// }