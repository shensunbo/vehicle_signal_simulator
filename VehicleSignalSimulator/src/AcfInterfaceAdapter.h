#ifndef __ACF_INTERFACE_ADAPTER__
#define __ACF_INTERFACE_ADAPTER__
#if 0
#include <cstdint>
#include <memory>
#include <string>

#include "VehicleDataServiceServer.h"
#include "aptiv/rtvsvm/RtvSvmService.hpp"

class AcfInterfaceAdapter {
public:
    AcfInterfaceAdapter() = delete;
    AcfInterfaceAdapter(std::string acf_vehicle_data_server_path, std::string acf_svm_service_client_path);
    ~AcfInterfaceAdapter();

    //acfServerInit first, hmiClientInit should be called after SvmApp run
    int acfServerInit();
    int hmiClientInit();
    bool isHmiClientAvailable();

//HMI cmd
    void SVM_HMI_Request_Debug_Cmd_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Active_SVM_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Touch_Action_Process(const uint8_t& data, const uint16_t& xPos, const uint8_t& yPos) const;
    void SVM_HMI_Request_Switch_View_Mode_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Active_Guideline_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Active_Radar_Icon_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Enable_Transparent_Car_Model_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Change_Car_Model_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle(const uint8_t& data) const;
    void SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle(const uint8_t& data) const;
    void SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action(const uint8_t& data) const;
    void SVM_HMI_Request_Start_EOL_Calibration_Process(const uint8_t& data) const;
    void SVM_HMI_Request_Start_Manual_Calibration_Process(const uint8_t& data) const;
    void SVM_HMI_Request_On_Video_Lost_Process(const uint8_t& data) const;
    void SVM_HMI_Select_SVM_View_Process(const uint8_t& data) const;

//vehicle data
    void SteeringWheelAngleProcess(const int32_t& data) const;
    void EachWheelPulseSignalProcess(const uint8_t& wheel, const uint8_t& pulse) const;
    void GearProcess(const uint8_t& data) const;
    void VehicleSpeedProcess(const uint8_t& data) const;
    void VehicleBodayHeightProcess(const uint8_t& data) const;
    void SteeringleversignalProcess(const uint8_t& data) const;
    void VehicleTypeProcess(const uint8_t& data) const;
    void VehicleVarintProcess(const uint8_t& data) const;
    void VehecleVINProcess(const uint32_t& data) const;
    void VehicleLanguageProcess(const uint8_t& data) const;
    void LeftFrontWindowOpenStateProcess(const uint8_t& data) const;
    void RightFrontWindowOpenStateProcess(const uint8_t& data) const;
    void LeftRearWindowOpenStateProcess(const uint8_t& data) const;
    void RightRearWindowOpenStateProcess(const uint8_t& data) const;
    void RunroofWindowOpenStateProcess(const uint8_t& data) const;
    void LeftFrontDoorOpenStateProcess(const uint8_t& data) const;
    void RightFrontDoorOpenStateProcess(const uint8_t& data) const;
    void LeftRearDoorOpenStateProcess(const uint8_t& data) const;
    void RightRearDoorOpenStateProcess(const uint8_t& data) const;
    void TrunkOpenStateProcess(const uint8_t& data) const;
    void HoodOpenStateProcess(const uint8_t& data) const;
    void HighbeamlightstateProcess(const uint8_t& data) const;
    void LowbeamheadlightstateProcess(const uint8_t& data) const;
    void FrontwheelprofilelightstateProcess(const uint8_t& data) const;
    void RearwheelprofilelightstateProcess(const uint8_t& data) const;
    void RearfoglightstateProcess(const uint8_t& data) const;
    void ReverselightstateProcess(const uint8_t& data) const;
    void BrakelightstateProcess(const uint8_t& data) const;
    void LeftturnsignalProcess(const uint8_t& data) const;
    void RightturnsignalProcess(const uint8_t& data) const;
    void DaytimerunninglightsProcess(const uint8_t& data) const;
    void ObstacleMinimumDistanceofeachradarProcess(const uint8_t& region, const uint8_t& distance) const;
    void WorkFaultStateofeachradarProcess(const uint8_t& data) const;
    void WarningStateofeachradarProcess(const uint8_t& data) const;
    void WarningLevelofeachradarProcess(const uint8_t& data) const;
    void APAworkingstateProcess(const uint8_t& data) const;
    void PEBwarningstateProcess(const uint8_t& data) const;
    void RCTAwarningProcess(const uint8_t& data) const;

private:
    //vehicle data
    std::shared_ptr<aptiv::vehicle_data::VehicleDataServiceServer> m_acf_server;
    std::string m_vehicle_data_server_cfg_path;
    acf::ServiceRegistry m_vehicle_data_server_registry;
    aptiv::AsyncExecutor m_vehicle_data_server_exec;
    std::vector<acf::Skeleton> m_vehicle_data_server_skeletons;

    //HMI cmd
    std::unique_ptr<aptiv::rtvsvm::RtvSvmService> m_hmi_client;
    std::string m_hmi_client_path;
    acf::ServiceRegistry m_hmi_client_registry;
    aptiv::AsyncExecutor m_hmi_client_exec;

};
#endif
#endif //#define __ACF_INTERFACE_ADAPTER_H__
