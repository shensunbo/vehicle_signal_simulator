#include "MessagelProccessor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#include "mylog.h"

SvmSignalProcessor::SvmSignalProcessor(std::string ip, int port):
    m_ip(ip),
    m_port(port),
    m_socket(-1),
    m_process_done(false),
    m_cmd_line_input_thread(nullptr)
{
}

SvmSignalProcessor::~SvmSignalProcessor() {
    mylog(LogLevel::I, "~SvmSignalProcessor() start");
    close(m_socket);

    if (m_cmd_line_input_thread != nullptr && m_cmd_line_input_thread->joinable()) {
        m_cmd_line_input_thread->join();
        delete m_cmd_line_input_thread;
        m_cmd_line_input_thread = nullptr;
    }

    mylog(LogLevel::I, "~SvmSignalProcessor() end");
}

/**
 * @brief Runs the SvmSignalProcessor.
 * 
 * This function initializes UDP server, processes incoming data, and commands.
 */
void SvmSignalProcessor::run() {
    if(consoleCamImpl.init()){
        mylog(LogLevel::E, "consoleCamImpl init failed");
        MY_ASSERT(0, "consoleCamImpl init failed");
        return;
    }

    if(udpServerInit()){
        mylog(LogLevel::E, "udpServerInit failed");
        MY_ASSERT(0, "udpServerInit failed");
        return;
    }

    //create cmd line input processor thread
    auto cmdLineProcessor = [this]() { return this->consoleCamImpl.execute(); };
    m_cmd_line_input_thread =  new std::thread(cmdLineProcessor);

    int bufsize = sizeof(SvmSignalData);
    char buf[bufsize] = {0};

    while(1) {
        if(m_process_done) {
            break;
        }

        memset(buf, 0, bufsize);
        int len = recvfrom(m_socket, buf, bufsize, 0, NULL, NULL);
        if(len > 0) {
            if(udpDatagramParse(buf, len)) {
                mylog(LogLevel::E, "udpDatagramParse failed");
                continue;
            }

            if(cmdProcess()){
                mylog(LogLevel::E, "cmdProcess failed");
                continue;
            }
        }
    }
}

void SvmSignalProcessor::end() {
    m_process_done = true;
    mylog(LogLevel::I, "SvmSignalProcessor stop");
}

int SvmSignalProcessor::udpServerInit() {
    int socket_fd;
    struct sockaddr_in server_addr;
    if ((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        mylog(LogLevel::E, "Socket creation failed");
        MY_ASSERT(0, "Socket creation failed");
        return -1;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = inet_addr(m_ip.c_str());

    if (bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0) {
        mylog(LogLevel::E, "Bind failed");
        MY_ASSERT(0, "Bind failed");
        return -1;
    }

    m_socket = socket_fd;

    mylog(LogLevel::I, "Socket created successfully IP: %s: port: %d: fd: %d",  m_ip.c_str(), m_port, m_socket);
    return 0;
}

int SvmSignalProcessor::udpDatagramParse(const char* buf, int len) {
    m_svm_signal_data_recv.fid = buf[0];
    m_svm_signal_data_recv.cmd = buf[1];
    m_svm_signal_data_recv.value = buf[2];
    m_svm_signal_data_recv.data[0] = buf[3];
    m_svm_signal_data_recv.data[1] = buf[4];
    m_svm_signal_data_recv.data[2] = buf[5];
    m_svm_signal_data_recv.data[3] = buf[6];

    if(len > 7) {
        for(int i = 7; i < len; i++) {
            MY_ASSERT(i < sizeof(m_svm_signal_data_recv), "i >= sizeof(m_svm_signal_data_recv)");
            m_svm_signal_data_recv.reserve[i - 7] = buf[i];
        }
    }

    std::cout<<std::endl;
    mylog(LogLevel::D, "fid: 0x%x, cmd: 0x%x, value: 0x%x, data: 0x%x, 0x%x, 0x%x, 0x%x, position: 0x%x, 0x%x, 0x%x, 0x%x", m_svm_signal_data_recv.fid, m_svm_signal_data_recv.cmd, m_svm_signal_data_recv.value, 
    m_svm_signal_data_recv.data[0], m_svm_signal_data_recv.data[1], m_svm_signal_data_recv.data[2], m_svm_signal_data_recv.data[3],
    m_svm_signal_data_recv.reserve[0],m_svm_signal_data_recv.reserve[1],m_svm_signal_data_recv.reserve[2],m_svm_signal_data_recv.reserve[3]);

    // uint16_t tmp = 0;
    // memcpy(&tmp, m_svm_signal_data_recv.reserve, sizeof(tmp));
    // mylog(LogLevel::D, "X: %d",tmp);
    // memcpy(&tmp, &m_svm_signal_data_recv.reserve[2], sizeof(tmp));
    // mylog(LogLevel::D, "Y: %d",tmp);
    return 0;
}

int SvmSignalProcessor::cmdProcess() {
    int ret = 0;

    switch(m_svm_signal_data_recv.cmd) {
        case SVM_HMI_REQUEST_DEBUG_CMD:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_DEBUG_CMD");
            SVM_HMI_Request_Debug_Cmd_Process();
            break;
        case SVM_HMI_REQUEST_ACTIVE_SVM:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ACTIVE_SVM");
            SVM_HMI_Request_Active_SVM_Process();
            break;
        case SVM_HMI_REQUEST_TOUCH_ACTION:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_TOUCH_ACTION");
            SVM_HMI_Request_Touch_Action_Process();
            break;
        case SVM_HMI_REQUEST_SWITCH_VIEW_MODE:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_SWITCH_VIEW_MODE");
            SVM_HMI_Request_Switch_View_Mode_Process();
            break;
        case SVM_HMI_REQUEST_ACTIVE_GUIDELINE:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ACTIVE_GUIDELINE");
            SVM_HMI_Request_Active_Guideline_Process();
            break;
        case SVM_HMI_REQUEST_ACTIVE_RADAR_ICON:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ACTIVE_RADAR_ICON");
            SVM_HMI_Request_Active_Radar_Icon_Process();
            break;
        case SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL");
            SVM_HMI_Request_Enable_Transparent_Car_Model_Process();
            break;
        case SVM_HMI_REQUEST_CHANGE_CAR_MODEL:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_CHANGE_CAR_MODEL");
            SVM_HMI_Request_Change_Car_Model_Process();
            break;
        case SVM_HMI_REQUEST_ENABLE_AUTO_SWITCH_VIEW_BY_OBSTACLE:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ENABLE_AUTO_SWITCH_VIEW_BY_OBSTACLE");
            SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle();
            break;
        case SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_OBSTACLE:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_OBSTACLE");
            SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle();
            break;
        case SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_TURN_ACTION:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_TURN_ACTION");
            SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action();
            break;
        case SVM_HMI_REQUEST_START_EOL_CALIBRATION:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_START_EOL_CALIBRATION");
            SVM_HMI_Request_Start_EOL_Calibration_Process();
            break;
        case SVM_HMI_REQUEST_START_MANUAL_CALIBRATION:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_START_MANUAL_CALIBRATION");
            SVM_HMI_Request_Start_Manual_Calibration_Process();
            break;
        case SVM_HMI_REQUEST_ON_VIDEO_LOST:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ON_VIDEO_LOST");
            SVM_HMI_Request_On_Video_Lost_Process();
            break;
        case SVM_HMI_SELECT_SVM_VIEW:
            mylog(LogLevel::D, "SVM_HMI_SELECT_SVM_VIEW");
            SVM_HMI_Select_SVM_View_Process();
            break;
        case STEERING_WHEEL_ANGLE:
            mylog(LogLevel::D, "STEERING_WHEEL_ANGLE");
            SteeringWheelAngleProcess();
            break;
        case EACH_WHEEL_PULSE_SIGNAL:
            mylog(LogLevel::D, "EACH_WHEEL_PULSE_SIGNAL");
            EachWheelPulseSignalProcess();
            break;
        case GEAR:
            mylog(LogLevel::D, "GEAR");
            GearProcess();
            break;
        case VEHICLE_SPEED:
            mylog(LogLevel::D, "VEHICLE_SPEED");
            VehicleSpeedProcess();
            break;
        case VEHICLE_BODAY_HEIGHT:
            mylog(LogLevel::D, "VEHICLE_BODAY_HEIGHT");
            VehicleBodayHeightProcess();
            break;
        case STEERING_LEVER_SIGNAL:
            mylog(LogLevel::D, "STEERING_LEVER_SIGNAL");
            SteeringleversignalProcess();
            break;
        case VEHICLE_TYPE:
            mylog(LogLevel::D, "VEHICLE_TYPE");
            VehicleTypeProcess();
            break;
        case VEHICLE_VARINT:
            mylog(LogLevel::D, "VEHICLE_VARINT");
            VehicleVarintProcess();
            break;
        case VEHICLE_VIN:
            mylog(LogLevel::D, "VEHICLE_VIN");
            VehecleVINProcess();
            break;
        case VEHICLE_LANGUAGE:
            mylog(LogLevel::D, "VEHICLE_LANGUAGE");
            VehicleLanguageProcess();
            break;
        case LEFT_FRONT_WINDOW_OPEN_STATE:
            mylog(LogLevel::D, "LEFT_FRONT_WINDOW_OPEN_STATE");
            LeftFrontWindowOpenStateProcess();
            break;
        case RIGHT_FRONT_WINDOW_OPEN_STATE:
            mylog(LogLevel::D, "RIGHT_FRONT_WINDOW_OPEN_STATE");
            RightFrontWindowOpenStateProcess();
            break;
        case LEFT_REAR_WINDOW_OPEN_STATE:
            mylog(LogLevel::D, "LEFT_REAR_WINDOW_OPEN_STATE");
            LeftRearWindowOpenStateProcess();
            break;
        case RIGHT_REAR_WINDOW_OPEN_STATE:
            mylog(LogLevel::D, "RIGHT_REAR_WINDOW_OPEN_STATE");
            RightRearWindowOpenStateProcess();
            break;
        case RUNROOF_WINDOW_OPEN_STATE:
            mylog(LogLevel::D, "RUNROOF_WINDOW_OPEN_STATE");
            RunroofWindowOpenStateProcess();
            break;
        case LEFT_FRONT_DOOR_OPEN_STATE:
            mylog(LogLevel::D, "LEFT_FRONT_DOOR_OPEN_STATE");
            LeftFrontDoorOpenStateProcess();
            break;
        case RIGHT_FRONT_DOOR_OPEN_STATE:
            mylog(LogLevel::D, "RIGHT_FRONT_DOOR_OPEN_STATE");
            RightFrontDoorOpenStateProcess();
            break;
        case LEFT_REAR_DOOR_OPEN_STATE:
            mylog(LogLevel::D, "LEFT_REAR_DOOR_OPEN_STATE");
            LeftRearDoorOpenStateProcess();
            break;
        case RIGHT_REAR_DOOR_OPEN_STATE:
            mylog(LogLevel::D, "RIGHT_REAR_DOOR_OPEN_STATE");
            RightRearDoorOpenStateProcess();
            break;
        case TRUNK_OPEN_STATE:
            mylog(LogLevel::D, "TRUNK_OPEN_STATE");
            TrunkOpenStateProcess();
            break;
        case HOOD_OPEN_STATE:
            mylog(LogLevel::D, "HOOD_OPEN_STATE");
            HoodOpenStateProcess();
            break;
        case HIGH_BEAM_LIGHT_STATE:
            mylog(LogLevel::D, "HIGH_BEAM_LIGHT_STATE");
            HighbeamlightstateProcess();
            break;
        case LOW_BEAM_HEADLIGHT_STATE:
            mylog(LogLevel::D, "LOW_BEAM_HEADLIGHT_STATE");
            LowbeamheadlightstateProcess();
            break;
        case FRONT_WHEEL_PROFILE_LIGHT_STATE:
            mylog(LogLevel::D, "FRONT_WHEEL_PROFILE_LIGHT_STATE");
            FrontwheelprofilelightstateProcess();
            break;
        case REAR_WHEEL_PROFILE_LIGHT_STATE:
            mylog(LogLevel::D, "REAR_WHEEL_PROFILE_LIGHT_STATE");
            RearwheelprofilelightstateProcess();
            break;
        case REAR_FOG_LIGHT_STATE:
            mylog(LogLevel::D, "REAR_FOG_LIGHT_STATE");
            RearfoglightstateProcess();
            break;
        case REVERSE_LIGHT_STATE:
            mylog(LogLevel::D, "REVERSE_LIGHT_STATE");
            ReverselightstateProcess();
            break;
        case BRAKE_LIGHT_STATE:
            mylog(LogLevel::D, "BRAKE_LIGHT_STATE");
            BrakelightstateProcess();
            break;
        case LEFT_TURN_SIGNAL:
            mylog(LogLevel::D, "LEFT_TURN_SIGNAL");
            LeftturnsignalProcess();
            break;
        case RIGHT_TURN_SIGNAL:
            mylog(LogLevel::D, "RIGHT_TURN_SIGNAL");
            RightturnsignalProcess();
            break;
        case DAYTIME_RUNNING_LIGHTS:
            mylog(LogLevel::D, "DAYTIME_RUNNING_LIGHTS");
            DaytimerunninglightsProcess();
            break;
        case OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR:
            mylog(LogLevel::D, "OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR");
            ObstacleMinimumDistanceofeachradarProcess();
            break;
        case WORK_FAULT_STATE_OF_EACH_RADAR:
            mylog(LogLevel::D, "WORK_FAULT_STATE_OF_EACH_RADAR");
            WorkFaultStateofeachradarProcess();
            break;
        case WARNING_STATE_OF_EACH_RADAR:
            mylog(LogLevel::D, "WARNING_STATE_OF_EACH_RADAR");
            WarningStateofeachradarProcess();
            break;
        case WARNING_LEVEL_OF_EACH_RADAR:
            mylog(LogLevel::D, "WARNING_LEVEL_OF_EACH_RADAR");
            WarningLevelofeachradarProcess();
            break;
        case APA_WORKING_STATE:
            mylog(LogLevel::D, "APA_WORKING_STATE");
            APAworkingstateProcess();
            break;
        case PEB_WARNING_STATE:
            mylog(LogLevel::D, "PEB_WARNING_STATE");
            PEBwarningstateProcess();
            break;
        case RCTA_WARNING:
            mylog(LogLevel::D, "RCTA_WARNING");
            RCTAwarningProcess();
            break;
        case SIMULATOR_EXIT:
            mylog(LogLevel::I, "SIMULATOR_EXIT");
            end();
            break;
        default:
            ret = -1;
            mylog(LogLevel::E, "cmd not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "cmd not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }

    return ret;
}

void SvmSignalProcessor::SVM_HMI_Request_Debug_Cmd_Process()  const{
    //m_acf_adapter.SVM_HMI_Request_Debug_Cmd_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Active_SVM_Process()  const{
    //m_acf_adapter.SVM_HMI_Request_Active_SVM_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Touch_Action_Process()  const{
    uint16_t xPos = 0;
    uint16_t yPos = 0;
    memcpy(&xPos, &m_svm_signal_data_recv.reserve[0],sizeof(xPos));
    memcpy(&yPos, &m_svm_signal_data_recv.reserve[2],sizeof(yPos));
    //m_acf_adapter.SVM_HMI_Request_Touch_Action_Process(m_svm_signal_data_recv.value, xPos, yPos);
}
void SvmSignalProcessor::SVM_HMI_Request_Switch_View_Mode_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Switch_View_Mode_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Active_Guideline_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Active_Guideline_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Active_Radar_Icon_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Active_Radar_Icon_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Enable_Transparent_Car_Model_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Enable_Transparent_Car_Model_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Change_Car_Model_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Change_Car_Model_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle() const {
    //m_acf_adapter.SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle() const {
    //m_acf_adapter.SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action() const {
    //m_acf_adapter.SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Start_EOL_Calibration_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Start_EOL_Calibration_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_Start_Manual_Calibration_Process() const {
    //m_acf_adapter.SVM_HMI_Request_Start_Manual_Calibration_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Request_On_Video_Lost_Process() const {
    //m_acf_adapter.SVM_HMI_Request_On_Video_Lost_Process(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::SVM_HMI_Select_SVM_View_Process() const {
    //m_acf_adapter.SVM_HMI_Select_SVM_View_Process(m_svm_signal_data_recv.value);
}

void SvmSignalProcessor::SteeringWheelAngleProcess() const{
    int8_t radians = 0;
    int32_t degrees = 0;
    memcpy(&radians, &m_svm_signal_data_recv.value, sizeof(radians));
    mylog(LogLevel::D, "SteeringWheelAngleProcess %d", radians);
    degrees = radians * 57; //Radians to degrees
    //m_acf_adapter.SteeringWheelAngleProcess(degrees);
}

void SvmSignalProcessor::EachWheelPulseSignalProcess() const{
    mylog(LogLevel::D, "EachWheelPulseSignalProcess");
    //m_acf_adapter.EachWheelPulseSignalProcess(m_svm_signal_data_recv.value, m_svm_signal_data_recv.data[0]);
}
void SvmSignalProcessor::GearProcess() const{
    //m_acf_adapter.GearProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::VehicleSpeedProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehicleSpeedProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehicleSpeed_value: %d", value);
}
void SvmSignalProcessor::VehicleBodayHeightProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehicleBodayHeightProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehicleBodayHeight_value: %d", value);
}
void SvmSignalProcessor::SteeringleversignalProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.SteeringleversignalProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "Steeringleversignal_value: %d", value);
}
void SvmSignalProcessor::VehicleTypeProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehicleTypeProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehicleType_value: %d", value);
}
void SvmSignalProcessor::VehicleVarintProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehicleVarintProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehicleVarint_value: %d", value);
}
void SvmSignalProcessor::VehecleVINProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehecleVINProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehecleVIN_value: %d", value);
}
void SvmSignalProcessor::VehicleLanguageProcess() const{
    auto value = m_svm_signal_data_recv.value;
    //m_acf_adapter.VehicleLanguageProcess(m_svm_signal_data_recv.value);
    mylog(LogLevel::D, "VehicleLanguage_value: %d", value);
}
void SvmSignalProcessor::LeftFrontWindowOpenStateProcess() const{
    //m_acf_adapter.LeftFrontWindowOpenStateProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::RightFrontWindowOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RightFrontWindowOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightFrontWindowOpenStateProcess::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightFrontWindowOpenStateProcess::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "RightFrontWindowOpenStateProcess not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "RightFrontWindowOpenStateProcess not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::LeftRearWindowOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.LeftRearWindowOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftRearWindowOpenStateProcess::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftRearWindowOpenStateProcess::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "LeftRearWindowOpenStateProcess not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "LeftRearWindowOpenStateProcess not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RightRearWindowOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RightRearWindowOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightRearWindowOpenStateProcess::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightRearWindowOpenStateProcess::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "RightRearWindowOpenStateProcess not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "RightRearWindowOpenStateProcess not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RunroofWindowOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RunroofWindowOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RunroofWindowOpenStateProcess::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RunroofWindowOpenStateProcess::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "RunroofWindowOpenStateProcess not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "RunroofWindowOpenStateProcess not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::LeftFrontDoorOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.LeftFrontDoorOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftFrontDoorOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftFrontDoorOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "LeftFrontDoorOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "LeftFrontDoorOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RightFrontDoorOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RightFrontDoorOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightFrontDoorOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightFrontDoorOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "RightFrontDoorOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "RightFrontDoorOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::LeftRearDoorOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.LeftRearDoorOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "LeftRearDoorOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "LeftRearDoorOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "LeftRearDoorOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "LeftRearDoorOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RightRearDoorOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RightRearDoorOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "RightRearDoorOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "RightRearDoorOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "RightRearDoorOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "RightRearDoorOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::TrunkOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.TrunkOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "TrunkOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "TrunkOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "TrunkOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "TrunkOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::HoodOpenStateProcess() const{
    auto value = static_cast<Door_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.HoodOpenStateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Door_State_value::OPEN:
            mylog(LogLevel::D, "HoodOpenState::OPEN");
            break;
        case Door_State_value::CLOSE:
            mylog(LogLevel::D, "HoodOpenState::CLOSE");
            break;
        default:
            mylog(LogLevel::E, "HoodOpenState not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "HoodOpenState not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::HighbeamlightstateProcess() const{
    //m_acf_adapter.HighbeamlightstateProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::LowbeamheadlightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.LowbeamheadlightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Lowbeamheadlightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Lowbeamheadlightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Lowbeamheadlightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Lowbeamheadlightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::FrontwheelprofilelightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.FrontwheelprofilelightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Frontwheelprofilelightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Frontwheelprofilelightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Frontwheelprofilelightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Frontwheelprofilelightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RearwheelprofilelightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RearwheelprofilelightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Rearwheelprofilelightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Rearwheelprofilelightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Rearwheelprofilelightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Rearwheelprofilelightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RearfoglightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RearfoglightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Rearfoglightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Rearfoglightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Rearfoglightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Rearfoglightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::ReverselightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.ReverselightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Reverselightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Reverselightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Reverselightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Reverselightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    } 
}
void SvmSignalProcessor::BrakelightstateProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.BrakelightstateProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Brakelightstate::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Brakelightstate::OFF");
            break;
        default:
            mylog(LogLevel::E, "Brakelightstate not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Brakelightstate not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::LeftturnsignalProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.LeftturnsignalProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Leftturnsignal::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Leftturnsignal::OFF");
            break;
        default:
            mylog(LogLevel::E, "Leftturnsignal not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Leftturnsignal not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::RightturnsignalProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.RightturnsignalProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Rightturnsignal::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Rightturnsignal::OFF");
            break;
        default:
            mylog(LogLevel::E, "Rightturnsignal not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Rightturnsignal not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}
void SvmSignalProcessor::DaytimerunninglightsProcess() const{
    auto value = static_cast<Light_State_value>(m_svm_signal_data_recv.value);
    //m_acf_adapter.DaytimerunninglightsProcess(m_svm_signal_data_recv.value);
    switch(value) {
        case Light_State_value::OPEN:
            mylog(LogLevel::D, "Daytimerunninglights::ON");
            break;
        case Light_State_value::CLOSE:
            mylog(LogLevel::D, "Daytimerunninglights::OFF");
            break;
        default:
            mylog(LogLevel::E, "Daytimerunninglights not found 0x%x",  m_svm_signal_data_recv.value);
            MY_ASSERT(0, "Daytimerunninglights not found 0x%x", m_svm_signal_data_recv.value);
            break;
    }
}

//TODO:below not impelement
void SvmSignalProcessor::ObstacleMinimumDistanceofeachradarProcess() const{
    //m_acf_adapter.ObstacleMinimumDistanceofeachradarProcess(m_svm_signal_data_recv.value, m_svm_signal_data_recv.reserve[0]);
}
void SvmSignalProcessor::WorkFaultStateofeachradarProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.WorkFaultStateofeachradarProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::WarningStateofeachradarProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.WarningStateofeachradarProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::WarningLevelofeachradarProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.WarningLevelofeachradarProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::APAworkingstateProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.APAworkingstateProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::PEBwarningstateProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.PEBwarningstateProcess(m_svm_signal_data_recv.value);
}
void SvmSignalProcessor::RCTAwarningProcess() const{
    mylog(LogLevel::D, "not implement");
    //m_acf_adapter.RCTAwarningProcess(m_svm_signal_data_recv.value);
}