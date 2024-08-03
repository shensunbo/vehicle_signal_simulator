#ifndef __SVM_SIGNAL_PROCCESSOR__
#define __SVM_SIGNAL_PROCCESSOR__
#include <cstdint>
#include <string>
#include <netinet/in.h>
#include <thread>
#include "SvmSignalTypes.h"
// #include "AcfInterfaceAdapter.h"
#include "ConsoleCamImpl.h"

class SvmSignalProcessor{
    public:
        SvmSignalProcessor(std::string ip, int port);
        SvmSignalProcessor() = delete;
        ~SvmSignalProcessor();

        void run();
        void end();
    private:
        int udpServerInit();
        int udpDatagramParse(const char* buf, int len);
        int cmdProcess();

        //HMI cmd process
        void SVM_HMI_Request_Debug_Cmd_Process() const;
        void SVM_HMI_Request_Active_SVM_Process() const;
        void SVM_HMI_Request_Touch_Action_Process() const;
        void SVM_HMI_Request_Switch_View_Mode_Process() const;
        void SVM_HMI_Request_Active_Guideline_Process() const;
        void SVM_HMI_Request_Active_Radar_Icon_Process() const;
        void SVM_HMI_Request_Enable_Transparent_Car_Model_Process() const;
        void SVM_HMI_Request_Change_Car_Model_Process() const;
        void SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle() const;
        void SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle() const;
        void SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action() const;
        void SVM_HMI_Request_Start_EOL_Calibration_Process() const;
        void SVM_HMI_Request_Start_Manual_Calibration_Process() const;
        void SVM_HMI_Request_On_Video_Lost_Process() const;
        void SVM_HMI_Select_SVM_View_Process() const;

        //vehicle data process
        void SteeringWheelAngleProcess() const;
        void EachWheelPulseSignalProcess() const;
        void GearProcess() const;
        void VehicleSpeedProcess() const;
        void VehicleBodayHeightProcess() const;
        void SteeringleversignalProcess() const;
        void VehicleTypeProcess() const;
        void VehicleVarintProcess() const;
        void VehecleVINProcess() const;
        void VehicleLanguageProcess() const;
        void LeftFrontWindowOpenStateProcess() const;
        void RightFrontWindowOpenStateProcess() const;
        void LeftRearWindowOpenStateProcess() const;
        void RightRearWindowOpenStateProcess() const;
        void RunroofWindowOpenStateProcess() const;
        void LeftFrontDoorOpenStateProcess() const;
        void RightFrontDoorOpenStateProcess() const;
        void LeftRearDoorOpenStateProcess() const;
        void RightRearDoorOpenStateProcess() const;
        void TrunkOpenStateProcess() const;
        void HoodOpenStateProcess() const;
        void HighbeamlightstateProcess() const;
        void LowbeamheadlightstateProcess() const;
        void FrontwheelprofilelightstateProcess() const;
        void RearwheelprofilelightstateProcess() const;
        void RearfoglightstateProcess() const;
        void ReverselightstateProcess() const;
        void BrakelightstateProcess() const;
        void LeftturnsignalProcess() const;
        void RightturnsignalProcess() const;
        void DaytimerunninglightsProcess() const;
        void ObstacleMinimumDistanceofeachradarProcess() const;
        void WorkFaultStateofeachradarProcess() const;
        void WarningStateofeachradarProcess() const;
        void WarningLevelofeachradarProcess() const;
        void APAworkingstateProcess() const;
        void PEBwarningstateProcess() const;
        void RCTAwarningProcess() const;

    //tools
    bool checkHmiClientState();
    private:
        SvmSignalData m_svm_signal_data_recv;
        int m_socket;
        std::string m_ip;
        int m_port;
        bool m_process_done;

        // AcfInterfaceAdapter m_acf_adapter;
        ConsoleCamImpl consoleCamImpl;
        std::thread *m_cmd_line_input_thread;
};

#endif