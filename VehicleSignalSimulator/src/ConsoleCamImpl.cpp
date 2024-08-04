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
    Tr_Inf << "0: Vehicle Data Simulator\n";
    Tr_Inf << "255: exit\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl::printConsoleCameraHelp() {
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: HMI settings\n";
    Tr_Inf << "1: vehicle settings\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}


void ConsoleCamImpl:: printHmiSettingHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: set guideline \n";
    Tr_Inf << "1: set transparent mode \n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}

void ConsoleCamImpl:: printVehicleSettingHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: set Wheel and Gear\n";
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

void ConsoleCamImpl::printSetTransparentModeHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "1: Enable transparent car model\n";
    Tr_Inf << "2: Disable transparent car model\n";
    Tr_Inf << "255: back\n";
    Tr_Inf << "*********************************************************************\n\n";
}


void ConsoleCamImpl::printSetWheelAndGearHelp(){
    Tr_Inf << "*********************************************************************\n\n";
    Tr_Inf << "0: Steering Wheel Angle\n";
    Tr_Inf << "1: Each Wheel Pulse Signal\n";
    Tr_Inf << "2: Gear\n";
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
        L_HMI_SETTINGS= 0,
        L_VEHICLE_SETTINGS,
    };

    m_cmd_data_send.fid = 0x01;

    while (active) {
        printConsoleCameraHelp();
        getInputKey(key);

        switch (key) {
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

void ConsoleCamImpl::processHmiSetting(){
    int key = 0;
    bool active = true;
    enum {
        L_SET_GUIDELINE,
        L_SET_TRANSPARENT_MODE,
    };

    while (active) {
        printHmiSettingHelp();
        getInputKey(key);
        switch (key) {
            case L_SET_GUIDELINE:
                processSetGuideline();
                break;
            case L_SET_TRANSPARENT_MODE:
                processSetTransparentMode();
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
    };

    while (active) {
        printVehicleSettingHelp();
        getInputKey(key);
        switch (key) {
            case L_SET_WHEEL_AND_GEAR:
                processSetWheelAndGear();
                break;
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

void ConsoleCamImpl::processSetWheelAndGear(){
    int key = 0;
    bool active = true;
    enum {
        L_STEERING_WHEEL_ANGLE,
        L_EACH_WHEEL_PULSE_SIGNAL,
        L_GEAR,
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