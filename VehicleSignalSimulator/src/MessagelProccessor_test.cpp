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
#define WAIT_TIME (100)
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
    EXPECT_CALL(*mockInterfaceAdapter_, SVM_HMI_Request_Active_Guideline_Process(
        static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW)))
        .Times(1);
    
    cmdData.fid = 0x01;
    cmdData.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;
    cmdData.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
    ret = sendto(clientFd_, &cmdData, sizeof(cmdData), 0, (struct sockaddr *)&clientAddr_, sizeof(clientAddr_));
    EXPECT_NE(ret, -1);
    //TODO: this is not good, but if not wait, sometimes failed
    std::this_thread::sleep_for(std::chrono::milliseconds(WAIT_TIME));
}