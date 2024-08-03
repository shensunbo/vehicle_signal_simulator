#include "AcfInterfaceAdapter.h"
#include "SvmSignalTypes.h"
#include "mylog.h"
#include <iostream>
#if 0
AcfInterfaceAdapter::AcfInterfaceAdapter(std::string acf_vehicle_data_server_path, std::string acf_svm_service_client_path){
    m_vehicle_data_server_cfg_path = acf_vehicle_data_server_path;
    m_hmi_client_path = acf_svm_service_client_path;
    m_acf_server = std::make_shared<aptiv::vehicle_data::VehicleDataServiceServer>();
}

AcfInterfaceAdapter::~AcfInterfaceAdapter(){

}

int AcfInterfaceAdapter::acfServerInit() {
    std::cout << "AcfInterfaceAdapter::init m_vehicle_data_server_cfg_path: "<<m_vehicle_data_server_cfg_path<<std::endl;
    m_vehicle_data_server_registry = acf::ServiceRegistry::create(m_vehicle_data_server_cfg_path);

    m_vehicle_data_server_skeletons = m_vehicle_data_server_registry.registerService<aptiv::vehicle_data::VehicleData>(*(m_acf_server.get()), "VehicleData1", m_vehicle_data_server_exec);
    if (m_vehicle_data_server_skeletons.empty()) {
        std::cout << "\nFAILED\nExiting... \n";
        return -1;
    }

    return 0;
}

int AcfInterfaceAdapter::hmiClientInit() {
    std::cout << "AcfInterfaceAdapter::init m_hmi_client_path: "<<m_hmi_client_path<<std::endl;
    acf::ServiceRegistry m_hmi_client_registry = acf::ServiceRegistry::create(m_hmi_client_path);

    m_hmi_client = m_hmi_client_registry.tryGetService<aptiv::rtvsvm::RtvSvmService>("RtvSvmService1", m_hmi_client_exec);
    if (!m_hmi_client) {
        std::cerr << "\nAcfInterfaceAdapter::init Service 'aptiv::rtvsvm::RtvSvmService' instance 'SvmService1' is not available\n";
        return -1;
    }

    return 0;
}

bool AcfInterfaceAdapter::isHmiClientAvailable(){
    if(nullptr == m_hmi_client){
        return false;
    }

    return true;
}

void AcfInterfaceAdapter::SVM_HMI_Request_Debug_Cmd_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Debug_Cmd_Value>(data);
    switch(value) {
        case SVM_HMI_Request_Debug_Cmd_Value::DUMP_FRAMES:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::DUMP_FRAMES");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_DUMP_FRAMES);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::START_VIDEO_RECORDING:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::START_VIDEO_RECORDING");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_START_VIDEO_REC);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::STOP_VIDEO_RECORDING:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::STOP_VIDEO_RECORDING");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_STOP_VIDEO_REC);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::CAPTURE_SCREEN:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::CAPTURE_SCREEN");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_CAPTURE_SCREEN);
        case SVM_HMI_Request_Debug_Cmd_Value::USE_VIDEO_SIMULATION_CAPTURE:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::USE_VIDEO_SIMULATION_CAPTURE");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_USE_VIDEO_SIMULATION_CAPTURE);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::USE_PICTURE_SET_SIMULATION_CAPTURE:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::USE_PICTURE_SET_SIMULATION_CAPTURE");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_USE_PICTURE_SET_SIMULATION_CAPTURE);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::DUMP_MESSAGE:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::DUMP_MESSAGE");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_USE_DUMP_MSG);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::COPY_ALL_DEBUG_DATA_TO_USB:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::COPY_ALL_DEBUG_DATA_TO_USB");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_COPY_ALL_DEBUG_DATA_TO_USB);
            break;
        case SVM_HMI_Request_Debug_Cmd_Value::EXECUTE_REMOTE_COMMAND_STRING:
            mylog(LogLevel::D, "SVM_HMI_Request_Debug_Cmd_Value::EXECUTE_REMOTE_COMMAND_STRING");
            m_hmi_client->SvmRequestDbgCmd(aptiv::rtvsvm::Debug_Cmd::DEBUG_CMD_EXECUTE_REMOTE_CMD_STRING);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Debug_Cmd_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Debug_Cmd_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Active_SVM_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Active_SVM_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Active_SVM_Value::NO_COMMANDS:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_SVM_Value::NO_COMMANDS");
            break;
        case SVM_HMI_Request_Active_SVM_Value::ACTIVE:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_SVM_Value::ACTIVE");
            m_hmi_client->SvmControl(aptiv::rtvsvm::Svm_Control::SVM_CTRL_START);
            break;
        case SVM_HMI_Request_Active_SVM_Value::DEACTIVE:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_SVM_Value::DEACTIVE");
            m_hmi_client->SvmControl(aptiv::rtvsvm::Svm_Control::SVM_CTRL_STOP);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Active_SVM_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Active_SVM_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Touch_Action_Process(const uint8_t& data, const uint16_t& xPos, const uint8_t& yPos) const{
    auto value = static_cast<SVM_HMI_Request_Touch_Action_Value>(data);
    
    switch(value) {
        case SVM_HMI_Request_Touch_Action_Value::RELEASE:
            mylog(LogLevel::D, "SVM_HMI_Request_Touch_Action_Value::RELEASE");
            m_hmi_client->SvmRequestTouchAction(aptiv::rtvsvm::Touch_Actions::TOUCH_ACTION_RELEASE);
            // m_hmi_client->SvmRequestTouchX(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(xPos));
            // m_hmi_client->SvmRequestTouchY(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(yPos));
            break;
        case SVM_HMI_Request_Touch_Action_Value::PRESSED:
            mylog(LogLevel::D, "SVM_HMI_Request_Touch_Action_Value::PRESSED");
            m_hmi_client->SvmRequestTouchAction(aptiv::rtvsvm::Touch_Actions::TOUCH_ACTION_PRESSED);
            m_hmi_client->SvmRequestTouchX(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(xPos));
            m_hmi_client->SvmRequestTouchY(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(yPos));
            break;
        case SVM_HMI_Request_Touch_Action_Value::HOLD:
            mylog(LogLevel::D, "SVM_HMI_Request_Touch_Action_Value::HOLD");
            m_hmi_client->SvmRequestTouchAction(aptiv::rtvsvm::Touch_Actions::TOUCH_ACTION_HOLD);
            m_hmi_client->SvmRequestTouchX(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(xPos));
            m_hmi_client->SvmRequestTouchY(static_cast<aptiv::rtvsvm::Touch_Coordinate_Type>(yPos));
            break;
        case SVM_HMI_Request_Touch_Action_Value::RESERVED:
            mylog(LogLevel::D, "SVM_HMI_Request_Touch_Action_Value::RESERVED");
            m_hmi_client->SvmRequestTouchAction(aptiv::rtvsvm::Touch_Actions::TOUCH_ACTION_RESERVED);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Touch_Action_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Touch_Action_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Switch_View_Mode_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Switch_View_Mode_Value>(data);
    
    switch(value) {
        case SVM_HMI_Request_Switch_View_Mode_Value::NO_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::NO_REQUEST");
            break;
        case SVM_HMI_Request_Switch_View_Mode_Value::BEV_SINGLE_MODE:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::BEV_SINGLE_MODE");
            break;
        case SVM_HMI_Request_Switch_View_Mode_Value::BEV_3D_MODE:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::BEV_3D_MODE");
            break;
        case SVM_HMI_Request_Switch_View_Mode_Value::BEV_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::BEV_WHEEL_VIEW_MODE");
            break;
        case SVM_HMI_Request_Switch_View_Mode_Value::WIDE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::WIDE_VIEW_MODE");
            break;
        case SVM_HMI_Request_Switch_View_Mode_Value::FLOATING_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Request_Switch_View_Mode_Value::FLOATING_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_FLOATING_BEV_VIEW);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Switch_View_Mode_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Switch_View_Mode_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Active_Guideline_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Active_Guideline_Value>(data);
    switch(value) {
        case SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW");
            m_hmi_client->GuidelinesSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW");
            m_hmi_client->GuidelinesSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW");
            m_hmi_client->GuidelinesSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Guideline_Value::HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Guideline_Value::HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW");
            m_hmi_client->GuidelinesSetting().setValue(false);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Active_Guideline_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Active_Guideline_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Active_Radar_Icon_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Active_Radar_Icon_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW");
            m_hmi_client->RadarWallSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW");
            m_hmi_client->RadarWallSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW");
            m_hmi_client->RadarWallSetting().setValue(true);
            break;
        case SVM_HMI_Request_Active_Radar_Icon_Value::HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
            mylog(LogLevel::D, "SVM_HMI_Request_Active_Radar_Icon_Value::HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW");
            m_hmi_client->RadarWallSetting().setValue(false);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Active_Radar_Icon_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Active_Radar_Icon_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Enable_Transparent_Car_Model_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Enable_Transparent_Car_Model_Value>(data);
    
    switch(value) {
        case SVM_HMI_Request_Enable_Transparent_Car_Model_Value::NO_COMMNADS:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Transparent_Car_Model_Value::NO_COMMNADS");
            break;
        case SVM_HMI_Request_Enable_Transparent_Car_Model_Value::ENABLE_TRANSPARENT_CAR_MODEL:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Transparent_Car_Model_Value::ENABLE_TRANSPARENT_CAR_MODEL");
            m_hmi_client->TransparentViewSetting().setValue(true);
            break;
        case SVM_HMI_Request_Enable_Transparent_Car_Model_Value::DISABLE_TRANSPARENT_CAR_MODEL:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Transparent_Car_Model_Value::DISABLE_TRANSPARENT_CAR_MODEL");
            m_hmi_client->TransparentViewSetting().setValue(false);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Enable_Transparent_Car_Model_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Enable_Transparent_Car_Model_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Change_Car_Model_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Change_Car_Model_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_1:
            mylog(LogLevel::D, "SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_1");
            break;
        case SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_2:
            mylog(LogLevel::D, "SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_2");
            break;
        case SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_3:
            mylog(LogLevel::D, "SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_3");
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Change_Car_Model_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Change_Car_Model_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value>(data);
    
    switch(value) {
        case SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::DISABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::DISABLED");
            break;
        case SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::ENABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::ENABLED");
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::DISABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::DISABLED");
            break;
        case SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::ENABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::ENABLED");
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::DISABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::DISABLED");
            break;
        case SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::ENABLED:
            mylog(LogLevel::D, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::ENABLED");
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Start_EOL_Calibration_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Start_EOL_Calibration_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Start_EOL_Calibration_Value::NO_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Start_EOL_Calibration_Value::NO_REQUEST");
            m_hmi_client->SvmRequestToStartEOLCalibration(aptiv::rtvsvm::Eol_Calib::EOL_CALIB_NO_REQUEST);
            break;
        case SVM_HMI_Request_Start_EOL_Calibration_Value::START_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Start_EOL_Calibration_Value::START_REQUEST");
            m_hmi_client->SvmRequestToStartEOLCalibration(aptiv::rtvsvm::Eol_Calib::EOL_CALIB_START);
            break;
        case SVM_HMI_Request_Start_EOL_Calibration_Value::STOP_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Start_EOL_Calibration_Value::STOP_REQUEST");
            m_hmi_client->SvmRequestToStartEOLCalibration(aptiv::rtvsvm::Eol_Calib::EOL_CALIB_STOP);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Start_EOL_Calibration_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Start_EOL_Calibration_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_Start_Manual_Calibration_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_Manual_Calibration_Value>(data);

    switch(value) {
        case SVM_HMI_Request_Manual_Calibration_Value::NO_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Manual_Calibration_Value::NO_REQUEST");
            m_hmi_client->SvmRequestToStartManualCalibration(aptiv::rtvsvm::Manual_Calib::MANUAL_CALIB_NO_REQUEST);
            break;
        case SVM_HMI_Request_Manual_Calibration_Value::START_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Manual_Calibration_Value::START_REQUEST");
            m_hmi_client->SvmRequestToStartManualCalibration(aptiv::rtvsvm::Manual_Calib::MANUAL_CALIB_START);
            break;
        case SVM_HMI_Request_Manual_Calibration_Value::STOP_REQUEST:
            mylog(LogLevel::D, "SVM_HMI_Request_Manual_Calibration_Value::STOP_REQUEST");
            m_hmi_client->SvmRequestToStartManualCalibration(aptiv::rtvsvm::Manual_Calib::MANUAL_CALIB_STOP);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_Manual_Calibration_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_Manual_Calibration_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Request_On_Video_Lost_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Request_On_Video_Lost_Value>(data);

    switch(value) {
        case SVM_HMI_Request_On_Video_Lost_Value::NOMAL:
            mylog(LogLevel::D, "SVM_HMI_Request_On_Video_Lost_Value::NOMAL");
            break;
        case SVM_HMI_Request_On_Video_Lost_Value::VIDEO_LOST:
            mylog(LogLevel::D, "SVM_HMI_Request_On_Video_Lost_Value::VIDEO_LOST");
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Request_On_Video_Lost_Value not found 0x%x",  data);
            MY_ASSERT(0, "SVM_HMI_Request_On_Video_Lost_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SVM_HMI_Select_SVM_View_Process(const uint8_t& data) const{
    auto value = static_cast<SVM_HMI_Select_SVM_View_Value>(data);

    switch(value) {
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_FRONT_3D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_REAR_3D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_LEFT_3D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_RIGHT_3D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_FRONT_2D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_REAR_2D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_LEFT_2D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_RIGHT_2D_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_4_WHEELS_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_FRONT_WHEELS_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_BEV_COMBINED_REAR_WHEELS_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE");
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_FRONT_WIDE_VIEW);
            break;
        case SVM_HMI_Select_SVM_View_Value::SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_Select_SVM_View_Value::SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE");
            m_hmi_client->SetView(aptiv::rtvsvm::View_Types::VIEW_TYPE_REAR_WIDE_VIEW);
            break;
        default:
            mylog(LogLevel::E, "SVM_HMI_Select_SVM_View_Value not found 0x%x",  data);
            // MY_ASSERT(0, "SVM_HMI_Select_SVM_View_Value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::SteeringWheelAngleProcess(const int32_t& data) const{
    m_acf_server->m_steeringWheelAngleNotif.raise(data);
}
void AcfInterfaceAdapter::EachWheelPulseSignalProcess(const uint8_t& wheel, const uint8_t& pulse) const{
    aptiv::vehicle_data::WheelPulseState wheelPulse;
    wheelPulse.wheel = static_cast<aptiv::vehicle_data::WheelType>(wheel);
    wheelPulse.pulse = pulse;
    m_acf_server->m_eachWheelPulseSignalNotif.raise(wheelPulse);
}
void AcfInterfaceAdapter::GearProcess(const uint8_t& data) const{
    auto value = static_cast<Gear_value>(data);

    switch(value) {
        case Gear_value::P:
            mylog(LogLevel::D, "Gear_value::P");
            m_acf_server->m_gearNotif.raise(aptiv::vehicle_data::GearState::P);
            break;
        case Gear_value::R:
            mylog(LogLevel::D, "Gear_value::R");
            m_acf_server->m_gearNotif.raise(aptiv::vehicle_data::GearState::R);
            break;
        case Gear_value::N:
            mylog(LogLevel::D, "Gear_value::N");
            m_acf_server->m_gearNotif.raise(aptiv::vehicle_data::GearState::N);
            break;
        case Gear_value::D:
            mylog(LogLevel::D, "Gear_value::D");
            m_acf_server->m_gearNotif.raise(aptiv::vehicle_data::GearState::D);
            break;
        default:
            mylog(LogLevel::E, "Gear_value not found 0x%x",  data);
            MY_ASSERT(0, "Gear_value not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::VehicleSpeedProcess(const uint8_t& data) const{
    m_acf_server->m_vehicleSpeedNotif.raise(data);
}
void AcfInterfaceAdapter::VehicleBodayHeightProcess(const uint8_t& data) const{
    m_acf_server->m_vehicleBodayHeightNotif.raise(data);
}
void AcfInterfaceAdapter::SteeringleversignalProcess(const uint8_t& data) const{
    m_acf_server->m_steeringleversignalNotif.raise(data);
}
void AcfInterfaceAdapter::VehicleTypeProcess(const uint8_t& data) const{
    m_acf_server->m_vehicleTypeNotif.raise(data);
}
void AcfInterfaceAdapter::VehicleVarintProcess(const uint8_t& data) const{
    m_acf_server->m_vehicleVarintNotif.raise(data);
}
void AcfInterfaceAdapter::VehecleVINProcess(const uint32_t& data) const{
    m_acf_server->m_vehicleVinNotif.raise(data);
}
void AcfInterfaceAdapter::VehicleLanguageProcess(const uint8_t& data) const{
    m_acf_server->m_vehicleLanguageNotif.raise(data);
}
void AcfInterfaceAdapter::LeftFrontWindowOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftFrontWindowOpenState::OPEN");
            m_acf_server->m_leftFrontWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftFrontWindowOpenState::CLOSE");
            m_acf_server->m_leftFrontWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "LeftFrontWindowOpenState not found 0x%x",  data);
            MY_ASSERT(0, "LeftFrontWindowOpenState not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RightFrontWindowOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightFrontWindowOpenStateProcess::OPEN");
            m_acf_server->m_rightFrontWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightFrontWindowOpenStateProcess::CLOSE");
            m_acf_server->m_rightFrontWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "RightFrontWindowOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RightFrontWindowOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::LeftRearWindowOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftRearWindowOpenStateProcess::OPEN");
            m_acf_server->m_leftRearWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftRearWindowOpenStateProcess::CLOSE");
            m_acf_server->m_leftRearWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "LeftRearWindowOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "LeftRearWindowOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RightRearWindowOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightRearWindowOpenStateProcess::OPEN");
            m_acf_server->m_rightRearWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightRearWindowOpenStateProcess::CLOSE");
            m_acf_server->m_rightRearWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "RightRearWindowOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RightRearWindowOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RunroofWindowOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RunroofWindowOpenStateProcess::OPEN");
            m_acf_server->m_runroofWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RunroofWindowOpenStateProcess::CLOSE");
            m_acf_server->m_runroofWindowOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "RunroofWindowOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RunroofWindowOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::LeftFrontDoorOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftFrontDoorOpenStateProcess::OPEN");
            m_acf_server->m_leftFrontDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftFrontDoorOpenStateProcess::CLOSE");
            m_acf_server->m_leftFrontDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "LeftFrontDoorOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "LeftFrontDoorOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RightFrontDoorOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightFrontDoorOpenStateProcess::OPEN");
            m_acf_server->m_rightFrontDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightFrontDoorOpenStateProcess::CLOSE");
            m_acf_server->m_rightFrontDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "RightFrontDoorOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RightFrontDoorOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::LeftRearDoorOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftRearDoorOpenStateProcess::OPEN");
            m_acf_server->m_leftRearDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftRearDoorOpenStateProcess::CLOSE");
            m_acf_server->m_leftRearDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "LeftRearDoorOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "LeftRearDoorOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RightRearDoorOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightRearDoorOpenStateProcess::OPEN");
            m_acf_server->m_rightRearDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightRearDoorOpenStateProcess::CLOSE");
            m_acf_server->m_rightRearDoorOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "RightRearDoorOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RightRearDoorOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::TrunkOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "TrunkOpenStateProcess::OPEN");
            m_acf_server->m_trunkOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "TrunkOpenStateProcess::CLOSE");
            m_acf_server->m_trunkOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "TrunkOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "TrunkOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::HoodOpenStateProcess(const uint8_t& data) const{
    auto value = static_cast<Door_State_value>(data);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "HoodOpenStateProcess::OPEN");
            m_acf_server->m_hoodOpenStateNotif.raise(aptiv::vehicle_data::DoorState::OPEN);
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "HoodOpenStateProcess::CLOSE");
            m_acf_server->m_hoodOpenStateNotif.raise(aptiv::vehicle_data::DoorState::CLOSE);
            break;
        default:
            mylog(LogLevel::E, "HoodOpenStateProcess not found 0x%x",  data);
            MY_ASSERT(0, "HoodOpenStateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::HighbeamlightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Highbeamlightstate::ON");
            m_acf_server->m_highbeamlightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Highbeamlightstate::OFF");
            m_acf_server->m_highbeamlightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "Highbeamlightstate not found 0x%x",  data);
            MY_ASSERT(0, "Highbeamlightstate not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::LowbeamheadlightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "LowbeamheadlightstateProcess::ON");
            m_acf_server->m_lowbeamheadlightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "LowbeamheadlightstateProcess::OFF");
            m_acf_server->m_lowbeamheadlightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "LowbeamheadlightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "LowbeamheadlightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::FrontwheelprofilelightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "FrontwheelprofilelightstateProcess::ON");
            m_acf_server->m_frontwheelprofilelightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "FrontwheelprofilelightstateProcess::OFF");
            m_acf_server->m_frontwheelprofilelightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "FrontwheelprofilelightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "FrontwheelprofilelightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RearwheelprofilelightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "RearwheelprofilelightstateProcess::ON");
            m_acf_server->m_rearwheelprofilelightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "RearwheelprofilelightstateProcess::OFF");
            m_acf_server->m_rearwheelprofilelightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "RearwheelprofilelightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RearwheelprofilelightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RearfoglightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "RearfoglightstateProcess::ON");
            m_acf_server->m_rearfoglightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "RearfoglightstateProcess::OFF");
            m_acf_server->m_rearfoglightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "RearfoglightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "RearfoglightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::ReverselightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "ReverselightstateProcess::ON");
            m_acf_server->m_reverselightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "ReverselightstateProcess::OFF");
            m_acf_server->m_reverselightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "ReverselightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "ReverselightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::BrakelightstateProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "BrakelightstateProcess::ON");
            m_acf_server->m_brakelightstateNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "BrakelightstateProcess::OFF");
            m_acf_server->m_brakelightstateNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "BrakelightstateProcess not found 0x%x",  data);
            MY_ASSERT(0, "BrakelightstateProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::LeftturnsignalProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "LeftturnsignalProcess::ON");
            m_acf_server->m_leftturnsignalNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "LeftturnsignalProcess::OFF");
            m_acf_server->m_leftturnsignalNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "LeftturnsignalProcess not found 0x%x",  data);
            MY_ASSERT(0, "LeftturnsignalProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::RightturnsignalProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "RightturnsignalProcess::ON");
            m_acf_server->m_rightturnsignalNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "RightturnsignalProcess::OFF");
            m_acf_server->m_rightturnsignalNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "RightturnsignalProcess not found 0x%x",  data);
            MY_ASSERT(0, "RightturnsignalProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::DaytimerunninglightsProcess(const uint8_t& data) const{
    auto value = static_cast<Light_State_value>(data);

    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "DaytimerunninglightsProcess::ON");
            m_acf_server->m_daytimerunninglightsNotif.raise(aptiv::vehicle_data::LightState::ON);
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "DaytimerunninglightsProcess::OFF");
            m_acf_server->m_daytimerunninglightsNotif.raise(aptiv::vehicle_data::LightState::OFF);
            break;
        default:
            mylog(LogLevel::E, "DaytimerunninglightsProcess not found 0x%x",  data);
            MY_ASSERT(0, "DaytimerunninglightsProcess not found 0x%x", data);
            break;
    }
}
void AcfInterfaceAdapter::ObstacleMinimumDistanceofeachradarProcess(const uint8_t& region, const uint8_t& distance) const{
    aptiv::vehicle_data::RadarState tmpRadarState;
    tmpRadarState.region = static_cast<aptiv::vehicle_data::RadarRegion>(region);
    tmpRadarState.distance = distance;
    
    m_acf_server->m_obstacleMinimumDistanceofeachradarNotif.raise(tmpRadarState);
}
void AcfInterfaceAdapter::WorkFaultStateofeachradarProcess(const uint8_t& data) const{
    m_acf_server->m_workFaultStateofeachradarNotif.raise(data);
}
void AcfInterfaceAdapter::WarningStateofeachradarProcess(const uint8_t& data) const{
    m_acf_server->m_warningStateofeachradarNotif.raise(data);
}
void AcfInterfaceAdapter::WarningLevelofeachradarProcess(const uint8_t& data) const{
    m_acf_server->m_warningLevelofeachradarNotif.raise(data);
}
void AcfInterfaceAdapter::APAworkingstateProcess(const uint8_t& data) const{
    m_acf_server->m_apaWorkingstateNotif.raise(data);
}
void AcfInterfaceAdapter::PEBwarningstateProcess(const uint8_t& data) const{
    m_acf_server->m_rctaWarningNotif.raise(data);
}
void AcfInterfaceAdapter::RCTAwarningProcess(const uint8_t& data) const{
    m_acf_server->m_rctaWarningNotif.raise(data);
}

#endif