#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <cstdlib>
#include <limits>
#include <unordered_map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#include "ConsoleCamImpl.h"
#include "mylog.h"

#define CONSOLE_CAM_EXIT  (255)
#define CONSOLE_CAM_BACK  (255)


#undef Tr_Inf
#define Tr_Inf std::cout

ConsoleCamImpl::ConsoleCamImpl():
    m_console_status(false),
    m_client_fd(-1),
    m_client_addr(),
    m_cmd_data_send()
{
}

ConsoleCamImpl::~ConsoleCamImpl() {
    close(m_client_fd);
}

int ConsoleCamImpl::init(){
    if ((m_client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        mylog(LogLevel::E, "Socket creation failed");
        MY_ASSERT(0, "Socket creation failed");
        return -1;
    }

    m_client_addr.sin_family = AF_INET;
    m_client_addr.sin_port = htons(PORT);
    m_client_addr.sin_addr.s_addr = inet_addr(CLIENT_IP.c_str());

    m_console_status = true;

    return 0;
}

void ConsoleCamImpl::execute() {
    MY_ASSERT(m_console_status, "m_console_status not init");

    pthread_t thread_id = pthread_self();
    (void)pthread_setname_np(thread_id, "ConsoleCamImpl::ConsoleCamImpl");
    mylog(LogLevel::I,"new thread Thread ID: %ld, Thread Name: ConsoleCamImpl::ConsoleCamImpl\n",thread_id);

    int key = 0;
    printHelp();
    while (m_console_status) {
        if (std::cin >> key) {
            // std::system("clear");
            processKey(key);
            fflush(stdin);
        } else {
            std::cin.clear();
            std::cin.ignore();
        }
    }

    m_cmd_data_send.fid = 0x01;
    m_cmd_data_send.cmd = SIMULATOR_EXIT;
    sendCurrentCmd();
}

void ConsoleCamImpl::sendCurrentCmd(){
    if(-1 == sendto(m_client_fd, &m_cmd_data_send, sizeof(m_cmd_data_send), 0, (struct sockaddr *)&m_client_addr, sizeof(m_client_addr))){
       Tr_Inf << "sendto failed "<< "cmd = " << m_cmd_data_send.cmd << " value = " << m_cmd_data_send.value << std::endl;
    }
}

void ConsoleCamImpl::printHelp() {
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Camera Command(AVM,PARK,PARKING)\n";
    Tr_Inf << "255: exit\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printConsoleCameraHelp() {
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: debug cmd\n";
    Tr_Inf << "1: active SVM service\n";
    Tr_Inf << "2: deactive SVM service\n";
    Tr_Inf << "3: touch event \n";
    Tr_Inf << "4: HMI settings\n";
    Tr_Inf << "5: vehicle settings\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printDebugCmdHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Dump frames\n";
    Tr_Inf << "1: Start Video Recording\n";
    Tr_Inf << "2: Stop Video Recording\n";
    Tr_Inf << "3: Capture Screen\n";
    Tr_Inf << "4: Use video simulation capture\n";
    Tr_Inf << "5: Use picture set simulation capture\n";
    Tr_Inf << "6: Dump Message\n";
    Tr_Inf << "7: Copy all debug data to USB\n";
    Tr_Inf << "8: Execute remote command string\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";

}

void ConsoleCamImpl:: printTouchEventHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Release\n";
    Tr_Inf << "1: Pressed\n";
    Tr_Inf << "2: Hold\n";
    Tr_Inf << "3: Request Touch X\n";
    Tr_Inf << "4: Request Touch Y\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl:: printHmiSettingHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: set view \n";
    Tr_Inf << "1: set guideline \n";
    Tr_Inf << "2: set radar mode\n";
    Tr_Inf << "3: set transparent mode \n";
    Tr_Inf << "4: change car model \n";
    Tr_Inf << "5: set auto active mode \n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl:: printVehicleSettingHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: set Wheel and Gear\n";
    Tr_Inf << "1: set Vehicle Config\n";
    Tr_Inf << "2: set Car Window and Door State\n";
    Tr_Inf << "3: set Car Light State\n";
    Tr_Inf << "4: set Radar State\n";
    Tr_Inf << "5: set Parking System\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetViewHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: set view mode\n";
    Tr_Inf << "1: set view direction\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetViewModeHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "1: BEV+Single Mode\n";
    Tr_Inf << "2: BEV+3D Mode\n";
    Tr_Inf << "3: BEV+Wheel View Mode\n";
    Tr_Inf << "4: Wide View Mode\n";
    Tr_Inf << "5: Floating View Mode\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetViewDirectionHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "1:  Select 3D front looking view under 3D view mode\n";
    Tr_Inf << "2:  Select 3D rear looking view under 3D view mode\n";
    Tr_Inf << "3:  Select 3D left looking view under 3D view mode\n";
    Tr_Inf << "4:  Select 3D right looking view under 3D view mode\n";
    Tr_Inf << "5:  Select 3D front-left looking view under 3D view mode\n";
    Tr_Inf << "6:  Select 3D front-right looking view under 3D view mode\n";
    Tr_Inf << "7:  Select 3D rear-left looking view under 3D view mode\n";
    Tr_Inf << "8:  Select 3D rear-right looking view under 3D view mode\n";
    Tr_Inf << "9:  Select 3D top looking view under 3D view mode\n";
    Tr_Inf << "10: Select front single view under single view mode\n";
    Tr_Inf << "11: Select rear single view under single view mode\n";
    Tr_Inf << "12: Select left single view under single view mode\n";
    Tr_Inf << "13: Select right single view under single view mode\n";
    Tr_Inf << "14: Select 4 wheels view under wheel view mode\n";
    Tr_Inf << "15: Select front 2 wheels view under wheel view mode\n";
    Tr_Inf << "16: Select rear 2 wheels view under wheel view mode\n";
    Tr_Inf << "17: Select left-front wheel view under wheel view mode\n";
    Tr_Inf << "18: Select right-front wheel view under wheel view mode\n";
    Tr_Inf << "19: Select left-rear wheel view under wheel view mode\n";
    Tr_Inf << "20: Select right-rear wheel view under wheel view mode\n";
    Tr_Inf << "21: Select front wide view under wide view mode\n";
    Tr_Inf << "22: Select rear wide view under wide view mode\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";

}
void ConsoleCamImpl::printSetGuidelineHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Show guideline both in bird eye view and single view\n";
    Tr_Inf << "1: Show guideline only in bird eye view \n";
    Tr_Inf << "2: Show guideline only in single view\n";
    Tr_Inf << "3: Hide guideline both in bird eye view and single view\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetRadarModeHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Show radar icon both in bird eye view and single view\n";
    Tr_Inf << "1: Show radar icon  only in bird eye view \n";
    Tr_Inf << "2: Show radar icon  only in single view\n";
    Tr_Inf << "3: Hide radar icon  both in bird eye view and single view\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetTransparentModeHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "1: Enable transparent car model\n";
    Tr_Inf << "2: Disable transparent car model\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printChangeCarModelHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Car model 1\n";
    Tr_Inf << "1: Car model 2\n";
    Tr_Inf << "2: Car model 3\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetAutoActiveModeHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: enables  auto switch view by obstacle\n";
    Tr_Inf << "1: disables auto switch view by obstacle \n";
    Tr_Inf << "2: enables  auto active view by obstacle \n";
    Tr_Inf << "3: disables auto active view by obstacle \n";
    Tr_Inf << "4: enables  auto active view by vehicle turn action \n";
    Tr_Inf << "5: disables auto active view by vehicle turn action \n";
    Tr_Inf << "6: start to enter EOL calibration\n";
    Tr_Inf << "7: stop EOL calibration\n";
    Tr_Inf << "8: start enter manual calibration\n";
    Tr_Inf << "9: stop manual calibration\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetWheelAndGearHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Steering Wheel Angle\n";
    Tr_Inf << "1: Each Wheel Pulse Signal\n";
    Tr_Inf << "2: Gear\n";
    Tr_Inf << "3: Vehicle Speed \n";
    Tr_Inf << "4: Vehicle Boday Height\n";
    Tr_Inf << "5: Steering lever signal\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetSteerWheelAngleHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: -540°\n";
    Tr_Inf << "1: -480°\n";
    Tr_Inf << "2: -420°\n";
    Tr_Inf << "3: -360°\n";
    Tr_Inf << "4: -300°\n";
    Tr_Inf << "5: -240°\n";
    Tr_Inf << "6: -180°\n";
    Tr_Inf << "7: -120°\n";
    Tr_Inf << "8: -60°\n";
    Tr_Inf << "9: 0°\n";
    Tr_Inf << "10: 60°\n";
    Tr_Inf << "11: 120°\n";
    Tr_Inf << "12: 180°\n";
    Tr_Inf << "13: 240°\n";
    Tr_Inf << "14: 300°\n";
    Tr_Inf << "15: 360°\n";
    Tr_Inf << "16: 420°\n";
    Tr_Inf << "17: 480°\n";
    Tr_Inf << "18: 540°\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printWheelPulseChooseWheelHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: front_left  wheel\n";
    Tr_Inf << "1: front_right wheel\n";
    Tr_Inf << "2: rear_left   wheel\n";
    Tr_Inf << "3: rear_right  wheel\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printWheelPulseSetPulseHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << " 0: 0\n";
    Tr_Inf << " 1: 25\n";
    Tr_Inf << " 2: 50\n";
    Tr_Inf << " 3: 75\n";
    Tr_Inf << " 4: 100\n";
    Tr_Inf << " 5: 125\n";
    Tr_Inf << " 6: 150\n";
    Tr_Inf << " 7: 175\n";
    Tr_Inf << " 8: 200\n";
    Tr_Inf << " 9: 225\n";
    Tr_Inf << "10: 250\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetGearHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << " 0: send P gear \n";
    Tr_Inf << " 1: send R gear\n";
    Tr_Inf << " 2: send N gear\n";
    Tr_Inf << " 3: send D gear\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetVehicleSpeedHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << " 0: 0 km/h\n";
    Tr_Inf << " 1: 10 km/h\n";
    Tr_Inf << " 2: 20 km/h\n";
    Tr_Inf << " 3: 30 km/h\n";
    Tr_Inf << " 4: 40 km/h\n";
    Tr_Inf << " 5: 50 km/h\n";
    Tr_Inf << " 6: 60 km/h\n";
    Tr_Inf << " 7: 70 km/h\n";
    Tr_Inf << " 8: 80 km/h\n";
    Tr_Inf << " 9: 90 km/h\n";
    Tr_Inf << "10: 100 km/h\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printSetVehicleConfigHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Vehicle Type\n";
    Tr_Inf << "1: Vehicle Varint\n";
    Tr_Inf << "2: VEHICLE VIN\n";
    Tr_Inf << "3: Vehicle Language\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetCarWindowAndDoorStateHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Left-Front Window Open\n";
    Tr_Inf << "1: Left-Front Window Close\n";
    Tr_Inf << "2: Right-Front Window Open\n";
    Tr_Inf << "3: Right-Front Window Close\n";
    Tr_Inf << "4: Left-Rear Window Open\n";
    Tr_Inf << "5: Left-Rear Window Close\n";
    Tr_Inf << "6: Right-Rear Window Open\n";
    Tr_Inf << "7: Right-Rear Window Close\n";
    Tr_Inf << "8: Runroof Window Open\n";
    Tr_Inf << "9: Runroof Window Close\n";
    Tr_Inf << "10: Left-Front Door Open\n";
    Tr_Inf << "11: Left-Front Door Close\n";
    Tr_Inf << "12: Right-Front Door Open\n";
    Tr_Inf << "13: Right-Front Door Close\n";
    Tr_Inf << "14: Left-Rear Door Open\n";
    Tr_Inf << "15: Left-Rear Door Close\n";
    Tr_Inf << "16: Right-Rear Door Open\n";
    Tr_Inf << "17: Right-Rear Door Close\n";
    Tr_Inf << "18: Trunk Open\n";
    Tr_Inf << "19: Trunk Close\n";
    Tr_Inf << "20: Hood Open\n";
    Tr_Inf << "21: Hood Close\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetCarLightStateHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: High beam light open\n";
    Tr_Inf << "1: High beam light close\n";
    Tr_Inf << "2: Low beam headlight open\n";
    Tr_Inf << "3: Low beam headlight close\n";
    Tr_Inf << "4: Front wheel profile light open\n";
    Tr_Inf << "5: Front wheel profile light close\n";
    Tr_Inf << "6: Rear wheel profile light open\n";
    Tr_Inf << "7: Rear wheel profile light close\n";
    Tr_Inf << "8: Rear fog light open\n";
    Tr_Inf << "9: Rear fog light close\n";
    Tr_Inf << "10: Reverse light open\n";
    Tr_Inf << "11: Reverse light close\n";
    Tr_Inf << "12: Brake light open \n";
    Tr_Inf << "13: Brake light close\n";
    Tr_Inf << "14: Left turn lights open \n";
    Tr_Inf << "15: Left turn lights close\n";
    Tr_Inf << "16: Right turn lights open \n";
    Tr_Inf << "17: Right turn lights close\n";
    Tr_Inf << "18: Daytime running lights open \n";
    Tr_Inf << "19: Daytime running lights close\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetRadarStateHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Obstacle Minimum Distance of each radar\n";
    Tr_Inf << "1: Work/Fault State of each radar\n";
    Tr_Inf << "2: Warning State of each radar\n";
    Tr_Inf << "3: Warning Level of each radar\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printChooseRadarHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0:  front_left_corner_region1\n";
    Tr_Inf << "1:  front_left_corner_region2\n";
    Tr_Inf << "2:  front_left_middle_region1\n";
    Tr_Inf << "3:  front_left_middle_region2\n";
    Tr_Inf << "4:  front_right_middle_region1\n";
    Tr_Inf << "5:  front_right_middle_region2\n";
    Tr_Inf << "6:  front_right_corner_region1\n";
    Tr_Inf << "7:  front_right_corner_region2\n";
    Tr_Inf << "8:  side_left_region1\n";
    Tr_Inf << "9:  side_left_region2\n";
    Tr_Inf << "10: side_left_region3\n";
    Tr_Inf << "11: side_left_region4\n";
    Tr_Inf << "12: side_left_region5\n";
    Tr_Inf << "13: side_right_region1\n";
    Tr_Inf << "14: side_right_region2\n";
    Tr_Inf << "15: side_right_region3\n";
    Tr_Inf << "16: side_right_region4\n";
    Tr_Inf << "17: side_right_region5\n";
    Tr_Inf << "18: rear_left_corner_region1\n";
    Tr_Inf << "19: rear_left_corner_region2\n";
    Tr_Inf << "20: rear_left_middle_region1\n";
    Tr_Inf << "21: rear_left_middle_region2\n";
    Tr_Inf << "22: rear_right_middle_region1\n";
    Tr_Inf << "23: rear_right_middle_region2\n";
    Tr_Inf << "24: rear_right_corner_region1\n";
    Tr_Inf << "25: rear_right_corner_region2\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetRadarDistanceHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0:  0 cm\n";
    Tr_Inf << "1:  10 cm\n";
    Tr_Inf << "2:  20 cm\n";
    Tr_Inf << "3:  30 cm\n";
    Tr_Inf << "4:  40 cm\n";
    Tr_Inf << "5:  60 cm\n";
    Tr_Inf << "6:  80 cm\n";
    Tr_Inf << "7:  100 cm\n";
    Tr_Inf << "8:  140 cm\n";
    Tr_Inf << "9:  180 cm\n";
    Tr_Inf << "10: 220 cm\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}
void ConsoleCamImpl::printSetParkingSystemHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: APA working state\n";
    Tr_Inf << "1: PEB warning state\n";
    Tr_Inf << "2: RCTA warning\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::processKey(int key) {
    switch (key) {
        case 0: {
            processConsoleCamera();
            break;
        }
        case CONSOLE_CAM_EXIT: {
            m_console_status = false;
            break;
        }
        default:
            break;
    }
}

void ConsoleCamImpl:: processConsoleCamera(){
    int key = 0;
    bool active = true;
    enum {
        L_DEBUG_CMD = 0,
        L_ACTIVE_SVM_SERVICE,
        L_DEACTIVE_SVM_SERVICE,
        L_TOUCH_EVENT,
        L_HMI_SETTINGS,
        L_VEHICLE_SETTINGS,
    };

    m_cmd_data_send.fid = 0x01;

    while (active) {
        printConsoleCameraHelp();
        getInputKey(key);

        switch (key) {
            case L_DEBUG_CMD:
                processDebugCmd();
                break;
            case L_ACTIVE_SVM_SERVICE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ACTIVE_SVM;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_SVM_Value::ACTIVE);
                sendCurrentCmd();
                break;
            case L_DEACTIVE_SVM_SERVICE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ACTIVE_SVM;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_SVM_Value::DEACTIVE);
                sendCurrentCmd();
                break;
            case L_TOUCH_EVENT:
                processTouchEvent();
                break;
            case L_HMI_SETTINGS:
                processHmiSetting();
                break;
            case L_VEHICLE_SETTINGS:
                processVehicleSetting();
                break;
            case CONSOLE_CAM_BACK:
                active = false;
                printHelp();
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processDebugCmd(){
    int key = 0;
    bool active = true;
    enum {
        L_DUMP_FRAMES = 0,
        L_START_VIDEO_RECORDING,
        L_STOP_VIDEO_RECORDING,
        L_CAPTURE_SCREEN,
        L_USE_VIDEO_SIMULATION_CAPTURE,
        L_USE_PICTURE_SET_SIMULATION_CAPTURE,
        L_DUMP_MESSAGE,
        L_COPY_ALL_DEBUG_DATA_TO_USB,
        L_EXECUTE_REMOTE_COMMAND_STRING,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_DEBUG_CMD;

    while (active) {
        printDebugCmdHelp();
        getInputKey(key);
        switch (key) {
            case L_DUMP_FRAMES:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::DUMP_FRAMES);
                sendCurrentCmd();
                break;
            case L_START_VIDEO_RECORDING:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::START_VIDEO_RECORDING);
                sendCurrentCmd();
                break;
            case L_STOP_VIDEO_RECORDING:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::STOP_VIDEO_RECORDING);
                sendCurrentCmd();
                break;
            case L_CAPTURE_SCREEN:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::CAPTURE_SCREEN);
                sendCurrentCmd();
                break;
            case L_USE_VIDEO_SIMULATION_CAPTURE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::USE_VIDEO_SIMULATION_CAPTURE);
                sendCurrentCmd();
                break;
            case L_USE_PICTURE_SET_SIMULATION_CAPTURE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::USE_PICTURE_SET_SIMULATION_CAPTURE);
                sendCurrentCmd();
                break;
            case L_DUMP_MESSAGE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::DUMP_MESSAGE);
                sendCurrentCmd();
                break;
            case L_COPY_ALL_DEBUG_DATA_TO_USB:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::COPY_ALL_DEBUG_DATA_TO_USB);
                sendCurrentCmd();
                break;
            case L_EXECUTE_REMOTE_COMMAND_STRING:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Debug_Cmd_Value::EXECUTE_REMOTE_COMMAND_STRING);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processTouchEvent(){
    int key = 0;
    bool active = true;
    enum {
        L_RELEASE = 0,
        L_PRESSED,
        L_HOLD,
        L_REQUEST_TOUCH_X,
        L_REQUEST_TOUCH_Y,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_TOUCH_ACTION;
    while (active) {
        printTouchEventHelp();
        getInputKey(key);
        switch (key) {
            case L_RELEASE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Touch_Action_Value::RELEASE);
                sendCurrentCmd();
                break;
            case L_PRESSED:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Touch_Action_Value::PRESSED);
                sendCurrentCmd();
                break;
            case L_HOLD:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Touch_Action_Value::HOLD);
                sendCurrentCmd();
                break;
            case L_REQUEST_TOUCH_X:
                Tr_Inf << "todo\n";
                break;
            case L_REQUEST_TOUCH_Y:
                Tr_Inf << "todo\n";
                break;
            case CONSOLE_CAM_BACK:
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processHmiSetting(){
    int key = 0;
    bool active = true;
    enum {
        L_SET_VIEW = 0,
        L_SET_GUIDELINE,
        L_SET_RADAR_MODE,
        L_SET_TRANSPARENT_MODE,
        L_CHANGE_CAR_MODEL,
        L_SET_AUTO_ACTIVE_MODE,
    };

    while (active) {
        printHmiSettingHelp();
        getInputKey(key);
        switch (key) {
            case L_SET_VIEW:
                processSetView();
                break;
            case L_SET_GUIDELINE:
                processSetGuideline();
                break;
            case L_SET_RADAR_MODE:
                processSetRadarMode();
                break;
            case L_SET_TRANSPARENT_MODE:
                processSetTransparentMode();
                break;
            case L_CHANGE_CAR_MODEL:
                processChangeCarModel();
                break;
            case L_SET_AUTO_ACTIVE_MODE:
                processSetAutoActiveMode();
                Tr_Inf << "set Auto Active Mode\n";
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processVehicleSetting(){
    int key = 0;
    bool active = true;
    enum {
        L_SET_WHEEL_AND_GEAR = 0,
        L_SET_VEHICLE_CONFIG,
        L_SET_CAR_WINDOW_AND_DOOR_STATE,
        L_SET_CAR_LIGHT_STATE,
        L_SET_RADAR_STATE,
        L_SET_PARKING_SYSTEM,
    };

    while (active) {
        printVehicleSettingHelp();
        getInputKey(key);
        switch (key) {
            case L_SET_WHEEL_AND_GEAR:
                processSetWheelAndGear();
                break;
            case L_SET_VEHICLE_CONFIG:
                Tr_Inf << "set Vehicle Config\n";
                processSetVehicleConfig();
                break;
            case L_SET_CAR_WINDOW_AND_DOOR_STATE:
                Tr_Inf << "set Car Window and Door State\n";
                processSetCarWindowAndDoorState();
                break;
            case L_SET_CAR_LIGHT_STATE:
                Tr_Inf << "set Car Light State\n";
                processSetCarLightState();
                break;
            case L_SET_RADAR_STATE:
                Tr_Inf << "set Radar State\n";
                processSetRadarState();
                break;
            case L_SET_PARKING_SYSTEM:
                Tr_Inf << "set Parking System\n";
                processSetParkingSystem();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processSetView(){
    int key = 0;
    bool active = true;
    enum {
        L_SET_VIEW_MODE = 0,
        L_SET_VIEW_DIRECTION,
    };

    while (active) {
        printSetViewHelp();
        getInputKey(key);
        switch (key) {
            case L_SET_VIEW_MODE:
                processSetViewMode();
                break;
            case L_SET_VIEW_DIRECTION:
                processSetViewDirection();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processSetViewMode(){
    int key = 0;
    bool active = true;
    enum {
        L_BEV_SINGLE_MODE = 1,
        L_BEV_3D_MODE,
        L_BEV_WHEEL_VIEW_MODE,
        L_WIDE_VIEW_MODE,
        L_FLOATING_VIEW_MODE,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_SWITCH_VIEW_MODE;

    while (active) {
        printSetViewModeHelp();
        getInputKey(key);
        switch (key) {
            case L_BEV_SINGLE_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Switch_View_Mode_Value::BEV_SINGLE_MODE);
                sendCurrentCmd();
                break;
            case L_BEV_3D_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Switch_View_Mode_Value::BEV_3D_MODE);
                sendCurrentCmd();
                break;
            case L_BEV_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Switch_View_Mode_Value::BEV_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_WIDE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Switch_View_Mode_Value::WIDE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_FLOATING_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Switch_View_Mode_Value::FLOATING_VIEW_MODE);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetViewDirection(){
    int key = 0;
    bool active = true;
    enum {
        L_SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE = 1,
        L_SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE,
        L_SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE,
        L_SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE,
        L_SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE,
        L_SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE,
        L_SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE,
        L_SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE,
        L_SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE,
    };

    m_cmd_data_send.cmd = SVM_HMI_SELECT_SVM_VIEW;

    while (active) {
        printSetViewDirectionHelp();
        getInputKey(key);
        switch (key) {
            case L_SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case L_SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Select_SVM_View_Value::SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetGuideline(){
    int key = 0;
    bool active = true;
    enum {
        L_SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW = 0,
        L_SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW,
        L_SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW,
        L_HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_ACTIVE_GUIDELINE;

    while (active) {
        printSetGuidelineHelp();
        getInputKey(key);
        switch (key) {
            case L_SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case L_SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW);
                sendCurrentCmd();
                break;
            case L_SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case L_HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Guideline_Value::HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetRadarMode(){
    int key = 0;
    bool active = true;
    enum {
        L_SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
        L_SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW,
        L_SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW,
        L_HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_ACTIVE_RADAR_ICON;

    while (active) {
        printSetRadarModeHelp();
        getInputKey(key);
        switch (key) {
            case L_SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case L_SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW);
                sendCurrentCmd();
                break;
            case L_SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Radar_Icon_Value::SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case L_HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Active_Radar_Icon_Value::HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetTransparentMode(){
    int key = 0;
    bool active = true;
    enum {
        L_ENABLE_TRANSPARENT_CAR_MODEL = 1,
        L_DISABLE_TRANSPARENT_CAR_MODEL,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL;

    while (active) {
        printSetTransparentModeHelp();
        getInputKey(key);
        switch (key) {
            case L_ENABLE_TRANSPARENT_CAR_MODEL:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::ENABLE_TRANSPARENT_CAR_MODEL);
                sendCurrentCmd();
                break;
            case L_DISABLE_TRANSPARENT_CAR_MODEL:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Transparent_Car_Model_Value::DISABLE_TRANSPARENT_CAR_MODEL);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processChangeCarModel(){
    int key = 0;
    bool active = true;
    enum {
        L_CAR_MODEL_1 = 0,
        L_CAR_MODEL_2,
        L_CAR_MODEL_3,
    };

    m_cmd_data_send.cmd = SVM_HMI_REQUEST_CHANGE_CAR_MODEL;

    while (active) {
        printChangeCarModelHelp();
        getInputKey(key);
        switch (key) {
            case L_CAR_MODEL_1:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_1);
                sendCurrentCmd();
                break;
            case L_CAR_MODEL_2:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_2);
                sendCurrentCmd();
                break;
            case L_CAR_MODEL_3:
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Change_Car_Model_Value::CAR_MODEL_3);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetAutoActiveMode(){
    int key = 0;
    bool active = true;
    enum {
        L_ENABLES_AUTO_SWITCH_VIEW_BY_OBSTACLE = 0,
        L_DISABLES_AUTO_SWITCH_VIEW_BY_OBSTACLE,
        L_ENABLES_AUTO_ACTIVE_VIEW_BY_OBSTACLE,
        L_DISABLES_AUTO_ACTIVE_VIEW_BY_OBSTACLE,
        L_ENABLES_AUTO_ACTIVE_VIEW_BY_VEHICLE_TURN_ACTION,
        L_DISABLES_AUTO_ACTIVE_VIEW_BY_VEHICLE_TURN_ACTION,
        L_START_TO_ENTER_EOL_CALIBRATION,
        L_STOP_EOL_CALIBRATION,
        L_START_ENTER_MANUAL_CALIBRATION,
        L_STOP_MANUAL_CALIBRATION,
    };

    while (active) {
        printSetAutoActiveModeHelp();
        getInputKey(key);
        switch (key) {
            case L_ENABLES_AUTO_SWITCH_VIEW_BY_OBSTACLE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_SWITCH_VIEW_BY_OBSTACLE;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::ENABLED);
                sendCurrentCmd();
                break;
            case L_DISABLES_AUTO_SWITCH_VIEW_BY_OBSTACLE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_SWITCH_VIEW_BY_OBSTACLE;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value::DISABLED);
                sendCurrentCmd();
                break;
            case L_ENABLES_AUTO_ACTIVE_VIEW_BY_OBSTACLE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_OBSTACLE;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::ENABLED);
                sendCurrentCmd();
                break;
            case L_DISABLES_AUTO_ACTIVE_VIEW_BY_OBSTACLE:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_OBSTACLE;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value::DISABLED);
                sendCurrentCmd();
                break;
            case L_ENABLES_AUTO_ACTIVE_VIEW_BY_VEHICLE_TURN_ACTION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_TURN_ACTION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::ENABLED);
                sendCurrentCmd();
                break;
            case L_DISABLES_AUTO_ACTIVE_VIEW_BY_VEHICLE_TURN_ACTION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_TURN_ACTION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value::DISABLED);
                sendCurrentCmd();
                break;
            case L_START_TO_ENTER_EOL_CALIBRATION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_START_EOL_CALIBRATION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Start_EOL_Calibration_Value::START_REQUEST);
                sendCurrentCmd();
                break;
            case L_STOP_EOL_CALIBRATION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_START_EOL_CALIBRATION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Start_EOL_Calibration_Value::STOP_REQUEST);
                sendCurrentCmd();
                break;
            case L_START_ENTER_MANUAL_CALIBRATION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_START_MANUAL_CALIBRATION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Manual_Calibration_Value::START_REQUEST);
                sendCurrentCmd();
                break;
            case L_STOP_MANUAL_CALIBRATION:
                m_cmd_data_send.cmd = SVM_HMI_REQUEST_START_MANUAL_CALIBRATION;
                m_cmd_data_send.value = static_cast<uint8_t>(SVM_HMI_Request_Manual_Calibration_Value::STOP_REQUEST);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetWheelAndGear(){
    int key = 0;
    bool active = true;
    enum {
        L_STEERING_WHEEL_ANGLE,
        L_EACH_WHEEL_PULSE_SIGNAL,
        L_GEAR,
        L_VEHICLE_SPEED,
        L_VEHICLE_BODAY_HEIGHT,
        L_STEERING_LEVER_SIGNAL,
    };

    while (active) {
        printSetWheelAndGearHelp();
        getInputKey(key);
        switch (key) {
            case L_STEERING_WHEEL_ANGLE:
                processSetSteerWheelAngle();    
                break;
            case L_EACH_WHEEL_PULSE_SIGNAL:
                processWheelPulseChooseWheel();
                break;
            case L_GEAR:
                processSetGear();
                break;
            case L_VEHICLE_SPEED:
                processSetVehicleSpeed();
                break;
            case L_VEHICLE_BODAY_HEIGHT:
                m_cmd_data_send.cmd = VEHICLE_BODAY_HEIGHT;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                Tr_Inf << "Not implemented yet\n";
                break;
            case L_STEERING_LEVER_SIGNAL:
                m_cmd_data_send.cmd = STEERING_LEVER_SIGNAL;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                Tr_Inf << "Not implemented yet\n";
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processSetSteerWheelAngle(){
    int key = 0;
    bool active = true;
    const std::unordered_map<int, int8_t> steerAngleMap = {
        {0,  -9},//-540°
        {1,  -8},
        {2,  -7},
        {3,  -6},
        {4,  -5},
        {5,  -4},
        {6,  -3},
        {7,  -2},
        {8,  -1},
        {9,  0},
        {10, 1},
        {11, 2},
        {12, 3},
        {13, 4},
        {14, 5},
        {15, 6},
        {16, 7},
        {17, 8},
        {18, 9},//540°
    };

    m_cmd_data_send.cmd = STEERING_WHEEL_ANGLE;
    m_cmd_data_send.value = 0;

    while (active) {
        printSetSteerWheelAngleHelp();
        getInputKey(key);

        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }

        if (steerAngleMap.find(key) != steerAngleMap.end()) {
            memcpy(&m_cmd_data_send.value, &steerAngleMap.at(key), sizeof(int8_t));
            sendCurrentCmd();
        }else{
            continue;
        }
    }
}

void ConsoleCamImpl::processWheelPulseChooseWheel(){
    int key = 0;
    bool active = true;

    const std::unordered_map<int, uint8_t> wheelIdMap = {
        {0,  0 }, //front_left  wheel
        {1,  1 }, //front_right wheel
        {2,  2 }, //rear_left   wheel
        {3,  3 }, //rear_right  wheel
    };

    while (active) {
        printWheelPulseChooseWheelHelp();
        getInputKey(key);

        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }

        if (wheelIdMap.find(key) != wheelIdMap.end()) {
            processWheelPulseSetPulse(wheelIdMap.at(key));
        }else{
            continue;
        }
    }
}

void ConsoleCamImpl::processWheelPulseSetPulse(const uint8_t wheelId){
    int key = 0;
    bool active = true;

    const std::unordered_map<int, uint8_t> pulseMap = {
        {0,  0  },
        {1,  25 },
        {2,  50 },
        {3,  75 },
        {4,  100},
        {5,  125},
        {6,  150},
        {7,  175},
        {8,  200},
        {9,  225},
        {10, 250},
    };

    m_cmd_data_send.cmd = EACH_WHEEL_PULSE_SIGNAL;
    m_cmd_data_send.value = wheelId;

    while (active) {
        printWheelPulseSetPulseHelp();
        getInputKey(key);

        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }

        if (pulseMap.find(key) != pulseMap.end()) {
            m_cmd_data_send.data[0] = pulseMap.at(key);
            sendCurrentCmd();
        }else{
            continue;
        }
    }
}

void ConsoleCamImpl::processSetGear(){
    int key = 0;
    bool active = true;
    enum {
        L_P = 0,
        L_R,
        L_N,
        L_D,
    }; 

    m_cmd_data_send.cmd = GEAR;

    while (active) {
        printSetGearHelp();
        getInputKey(key);
        switch (key) {
            case L_P:
                m_cmd_data_send.value = static_cast<uint8_t>(Gear_value::P);
                sendCurrentCmd();
                break;
            case L_R:
                m_cmd_data_send.value = static_cast<uint8_t>(Gear_value::R);
                sendCurrentCmd();
                break;
            case L_N:
                m_cmd_data_send.value = static_cast<uint8_t>(Gear_value::N);
                sendCurrentCmd();
                break;
            case L_D:
                m_cmd_data_send.value = static_cast<uint8_t>(Gear_value::D);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetVehicleSpeed(){
    int key = 0;
    bool active = true;
    const std::unordered_map<int, uint8_t> speedMap = {
        {0, 0},
        {1, 10},
        {2, 20},
        {3, 30},
        {4, 40},
        {5, 50},
        {6, 60},
        {7, 70},
        {8, 80},
        {9, 90},
        {10, 100}
    };

    m_cmd_data_send.cmd = VEHICLE_SPEED;

    while (active) {
        printSetVehicleSpeedHelp();
        getInputKey(key);
        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }
        if (speedMap.find(key) != speedMap.end()) {
            m_cmd_data_send.value = speedMap.at(key);
            sendCurrentCmd();
        }else{
            continue;
        }
    }

}

//TODO:not done
void ConsoleCamImpl::processSetVehicleConfig(){
    int key = 0;
    bool active = true;
    enum {
        L_VEHICLE_TYPE = 0,
        L_VEHICLE_VARINT,
        L_VEHICLE_VIN,
        L_VEHICLE_LANGUAGE,
    };

    while (active) {
        printSetVehicleConfigHelp();
        getInputKey(key);
        switch (key) {
            case L_VEHICLE_TYPE:
                m_cmd_data_send.cmd = VEHICLE_TYPE;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_VEHICLE_VARINT:
                m_cmd_data_send.cmd = VEHICLE_VARINT;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_VEHICLE_VIN:
                m_cmd_data_send.cmd = VEHICLE_VIN;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_VEHICLE_LANGUAGE:
                m_cmd_data_send.cmd = VEHICLE_LANGUAGE;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetCarWindowAndDoorState(){
    int key = 0;
    bool active = true;
    enum {
        L_LEFT_FRONT_WINDOW_OPEN = 0,
        L_LEFT_FRONT_WINDOW_CLOSE,
        L_RIGHT_FRONT_WINDOW_OPEN,
        L_RIGHT_FRONT_WINDOW_CLOSE,
        L_LEFT_REAR_WINDOW_OPEN,
        L_LEFT_REAR_WINDOW_CLOSE,
        L_RIGHT_REAR_WINDOW_OPEN,
        L_RIGHT_REAR_WINDOW_CLOSE,
        L_RUNROOF_WINDOW_OPEN,
        L_RUNROOF_WINDOW_CLOSE,
        L_LEFT_FRONT_DOOR_OPEN,
        L_LEFT_FRONT_DOOR_CLOSE,
        L_RIGHT_FRONT_DOOR_OPEN,
        L_RIGHT_FRONT_DOOR_CLOSE,
        L_LEFT_REAR_DOOR_OPEN,
        L_LEFT_REAR_DOOR_CLOSE,
        L_RIGHT_REAR_DOOR_OPEN,
        L_RIGHT_REAR_DOOR_CLOSE,
        L_TRUNK_OPEN,
        L_TRUNK_CLOSE,
        L_HOOD_OPEN,
        L_HOOD_CLOSE,
    };

    while (active) {
        printSetCarWindowAndDoorStateHelp();
        getInputKey(key);
        switch (key) {
            case L_LEFT_FRONT_WINDOW_OPEN:
                m_cmd_data_send.cmd = LEFT_FRONT_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LEFT_FRONT_WINDOW_CLOSE:
                m_cmd_data_send.cmd = LEFT_FRONT_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RIGHT_FRONT_WINDOW_OPEN:
                m_cmd_data_send.cmd = RIGHT_FRONT_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RIGHT_FRONT_WINDOW_CLOSE:
                m_cmd_data_send.cmd = RIGHT_FRONT_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_LEFT_REAR_WINDOW_OPEN:
                m_cmd_data_send.cmd = LEFT_REAR_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LEFT_REAR_WINDOW_CLOSE:
                m_cmd_data_send.cmd = LEFT_REAR_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RIGHT_REAR_WINDOW_OPEN:
                m_cmd_data_send.cmd = RIGHT_REAR_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RIGHT_REAR_WINDOW_CLOSE:
                m_cmd_data_send.cmd = RIGHT_REAR_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RUNROOF_WINDOW_OPEN:
                m_cmd_data_send.cmd = RUNROOF_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RUNROOF_WINDOW_CLOSE:
                m_cmd_data_send.cmd = RUNROOF_WINDOW_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_LEFT_FRONT_DOOR_OPEN:
                m_cmd_data_send.cmd = LEFT_FRONT_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LEFT_FRONT_DOOR_CLOSE:
                m_cmd_data_send.cmd = LEFT_FRONT_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RIGHT_FRONT_DOOR_OPEN:
                m_cmd_data_send.cmd = RIGHT_FRONT_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RIGHT_FRONT_DOOR_CLOSE:
                m_cmd_data_send.cmd = RIGHT_FRONT_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_LEFT_REAR_DOOR_OPEN:
                m_cmd_data_send.cmd = LEFT_REAR_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LEFT_REAR_DOOR_CLOSE:
                m_cmd_data_send.cmd = LEFT_REAR_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RIGHT_REAR_DOOR_OPEN:
                m_cmd_data_send.cmd = RIGHT_REAR_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RIGHT_REAR_DOOR_CLOSE:
                m_cmd_data_send.cmd = RIGHT_REAR_DOOR_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_TRUNK_OPEN:
                m_cmd_data_send.cmd = TRUNK_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_TRUNK_CLOSE:
                m_cmd_data_send.cmd = TRUNK_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_HOOD_OPEN:
                m_cmd_data_send.cmd = HOOD_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_HOOD_CLOSE:
                m_cmd_data_send.cmd = HOOD_OPEN_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Door_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
void ConsoleCamImpl::processSetCarLightState(){
    int key = 0;
    bool active = true;
    enum {
        L_HIGH_BEAM_LIGHT_OPEN = 0,
        L_HIGH_BEAM_LIGHT_CLOSE,
        L_LOW_BEAM_HEADLIGHT_OPEN,
        L_LOW_BEAM_HEADLIGHT_CLOSE,
        L_FRONT_WHEEL_PROFILE_LIGHT_OPEN,
        L_FRONT_WHEEL_PROFILE_LIGHT_CLOSE,
        L_REAR_WHEEL_PROFILE_LIGHT_OPEN,
        L_REAR_WHEEL_PROFILE_LIGHT_CLOSE,
        L_REAR_FOG_LIGHT_OPEN,
        L_REAR_FOG_LIGHT_CLOSE,
        L_REVERSE_LIGHT_OPEN,
        L_REVERSE_LIGHT_CLOSE,
        L_BRAKE_LIGHT_OPEN,
        L_BRAKE_LIGHT_CLOSE,
        L_LEFT_TURN_LIGHTS_OPEN,
        L_LEFT_TURN_LIGHTS_CLOSE,
        L_RIGHT_TURN_LIGHTS_OPEN,
        L_RIGHT_TURN_LIGHTS_CLOSE,
        L_DAYTIME_RUNNING_LIGHTS_OPEN,
        L_DAYTIME_RUNNING_LIGHTS_CLOSE,
    };

    while (active) {
        printSetCarLightStateHelp();
        getInputKey(key);
        switch (key) {
            case L_HIGH_BEAM_LIGHT_OPEN:
                m_cmd_data_send.cmd = HIGH_BEAM_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_HIGH_BEAM_LIGHT_CLOSE:
                m_cmd_data_send.cmd = HIGH_BEAM_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_LOW_BEAM_HEADLIGHT_OPEN:
                m_cmd_data_send.cmd = LOW_BEAM_HEADLIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LOW_BEAM_HEADLIGHT_CLOSE:
                m_cmd_data_send.cmd = LOW_BEAM_HEADLIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_FRONT_WHEEL_PROFILE_LIGHT_OPEN:
                m_cmd_data_send.cmd = FRONT_WHEEL_PROFILE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_FRONT_WHEEL_PROFILE_LIGHT_CLOSE:
                m_cmd_data_send.cmd = FRONT_WHEEL_PROFILE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_REAR_WHEEL_PROFILE_LIGHT_OPEN:
                m_cmd_data_send.cmd = REAR_WHEEL_PROFILE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_REAR_WHEEL_PROFILE_LIGHT_CLOSE:
                m_cmd_data_send.cmd = REAR_WHEEL_PROFILE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_REAR_FOG_LIGHT_OPEN:
                m_cmd_data_send.cmd = REAR_FOG_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_REAR_FOG_LIGHT_CLOSE:
                m_cmd_data_send.cmd = REAR_FOG_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_REVERSE_LIGHT_OPEN:
                m_cmd_data_send.cmd = REVERSE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_REVERSE_LIGHT_CLOSE:
                m_cmd_data_send.cmd = REVERSE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_BRAKE_LIGHT_OPEN:
                m_cmd_data_send.cmd = BRAKE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_BRAKE_LIGHT_CLOSE:
                m_cmd_data_send.cmd = BRAKE_LIGHT_STATE;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_LEFT_TURN_LIGHTS_OPEN:
                m_cmd_data_send.cmd = LEFT_TURN_SIGNAL;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_LEFT_TURN_LIGHTS_CLOSE:
                m_cmd_data_send.cmd = LEFT_TURN_SIGNAL;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_RIGHT_TURN_LIGHTS_OPEN:
                m_cmd_data_send.cmd = RIGHT_TURN_SIGNAL;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_RIGHT_TURN_LIGHTS_CLOSE:
                m_cmd_data_send.cmd = RIGHT_TURN_SIGNAL;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case L_DAYTIME_RUNNING_LIGHTS_OPEN:
                m_cmd_data_send.cmd = DAYTIME_RUNNING_LIGHTS;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::OPEN);
                sendCurrentCmd();
                break;
            case L_DAYTIME_RUNNING_LIGHTS_CLOSE:
                m_cmd_data_send.cmd = DAYTIME_RUNNING_LIGHTS;
                m_cmd_data_send.value = static_cast<uint8_t>(Light_State_value::CLOSE);
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}
//TODO:
void ConsoleCamImpl::processSetRadarState(){
    int key = 0;
    bool active = true;
    enum {
        L_OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR,
        L_STATE_OF_EACH_RADAR,
        L_WARNING_STATE_OF_EACH_RADAR,
        L_WARNING_LEVEL_OF_EACH_RADAR,
    };

    while (active) {    
        printSetRadarStateHelp();
        getInputKey(key);
        switch (key) {
            case L_OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR:
                processChooseRadar();
            case L_STATE_OF_EACH_RADAR:
                m_cmd_data_send.cmd = WORK_FAULT_STATE_OF_EACH_RADAR;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_WARNING_STATE_OF_EACH_RADAR:
                m_cmd_data_send.cmd = WARNING_STATE_OF_EACH_RADAR;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_WARNING_LEVEL_OF_EACH_RADAR:
                m_cmd_data_send.cmd = WARNING_LEVEL_OF_EACH_RADAR;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

void ConsoleCamImpl::processChooseRadar(){
    int key = 0;
    bool active = true;

    const std::unordered_map<int, uint8_t> RadarIdMap = {
        {0,  0 },
        {1,  1 },
        {2,  2 },
        {3,  3 },
        {4,  4 },
        {5,  5 },
        {6,  6 },
        {7,  7 },
        {8,  8 },
        {9,  9 },
        {10, 10},
        {11, 11},
        {12, 12},
        {13, 13},
        {14, 14},
        {15, 15},
        {16, 16},
        {17, 17},
        {18, 18},
        {19, 19},
        {20, 20},
        {21, 21},
        {22, 22},
        {23, 23},
        {24, 24},
        {25, 25}
    };

    while (active) {
        printChooseRadarHelp();
        getInputKey(key);

        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }

        if (RadarIdMap.find(key) != RadarIdMap.end()) {
            processSetRadarDistance(RadarIdMap.at(key));
        }else{
            continue;
        }
    }
}
void ConsoleCamImpl::processSetRadarDistance(const uint8_t radar_id){
    int key = 0;
    bool active = true;

    const std::unordered_map<int, uint8_t> RadarDistanceMap = {
        {0,  0},
        {1,  10},
        {2,  20},
        {3,  30},
        {4,  40},
        {5,  60},
        {6,  80},
        {7,  100},
        {8,  140},
        {9,  180},
        {10, 220},
    };

    m_cmd_data_send.cmd = OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR;
    m_cmd_data_send.value = radar_id;

    while (active) {
        printSetRadarDistanceHelp();
        getInputKey(key);

        if(CONSOLE_CAM_BACK == key){
            active = false;
            continue;
        }

        if (RadarDistanceMap.find(key) != RadarDistanceMap.end()) {
            m_cmd_data_send.reserve[0] = RadarDistanceMap.at(key);
            sendCurrentCmd();
        }else{
            continue;
        }
    }
}
//TODO:
void ConsoleCamImpl::processSetParkingSystem(){
    int key = 0;
    bool active = true;
    enum {
        L_APA_WORKING_STATE,
        L_PEB_WARNING_STATE,
        L_RCTA_WARNING,
    };

    while (active) {    
        printSetParkingSystemHelp();
        getInputKey(key);
        switch (key) {
            case L_APA_WORKING_STATE:
                m_cmd_data_send.cmd = APA_WORKING_STATE;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_PEB_WARNING_STATE:
                m_cmd_data_send.cmd = PEB_WARNING_STATE;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case L_RCTA_WARNING:
                m_cmd_data_send.cmd = RCTA_WARNING;
                m_cmd_data_send.value = 0;
                sendCurrentCmd();
                break;
            case CONSOLE_CAM_BACK: 
                active = false;
                break;
            default:
                break;
        }
    }
}

/**
 * @brief Retrieves an integer input from the console.
 * 
 * This function prompts the user to input an integer value from the console. 
 * If a non-integer value is entered, the function will continue prompting 
 * until a valid integer is provided.
 * 
 * @param key [out] Reference to an integer where the input value will be stored.
 */
void ConsoleCamImpl::getInputKey(int& key){
    while (!(std::cin >> key)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}