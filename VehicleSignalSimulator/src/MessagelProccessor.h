#pragma once

#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <thread>
#include <memory>
#include "SvmSignalTypes.h"
#include "ConsoleCamImpl.h"
#include "InterfaceAdapter.h"

class SvmSignalProcessor{
    public:
        SvmSignalProcessor(std::string ip, int port, std::shared_ptr<InterfaceAdapter> _interfaceAdapter);
        SvmSignalProcessor() = delete;
        ~SvmSignalProcessor();

        void clientRun();
        void serverRun();
        void end(); //only can be called by client, use SIMULATOR_EXIT message
    private:
        int udpServerInit();
        int udpDatagramParse(const char* buf, int len);
        int cmdProcess();

        //HMI cmd process
        void SVM_HMI_Request_Active_Guideline_Process() const;
        void SVM_HMI_Request_Enable_Transparent_Car_Model_Process() const;

        //vehicle data process
        void SteeringWheelAngleProcess() const;
        void EachWheelPulseSignalProcess() const;
        void GearProcess() const;

    //tools
    bool checkHmiClientState();
    private:
        SvmSignalData m_svm_signal_data_recv;
        int m_socket;
        std::string m_ip;
        int m_port;
        bool m_process_done;

        ConsoleCamImpl consoleCamImpl;
        std::thread *m_cmd_line_input_thread;

        std::shared_ptr<InterfaceAdapter> m_interface_adapter;
};