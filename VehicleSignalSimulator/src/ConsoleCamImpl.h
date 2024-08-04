#pragma once

#include "SvmSignalTypes.h"
#include <string>
#include <netinet/in.h>

class ConsoleCamImpl{
public:
    ConsoleCamImpl();
    ~ConsoleCamImpl();

    int init();
    void execute();
private:
    void sendCurrentCmd();

    //print help message
    void printHelp();
    void printConsoleCameraHelp();
    void printHmiSettingHelp();
    void printVehicleSettingHelp();
    void printSetGuidelineHelp();
    void printSetTransparentModeHelp();
    void printSetWheelAndGearHelp();
    void printSetSteerWheelAngleHelp();
    void printWheelPulseChooseWheelHelp();
    void printWheelPulseSetPulseHelp();
    void printSetGearHelp();

    //process HMI cmd
    void processKey(int key);
    void processConsoleCamera();
    void processHmiSetting();
    void processVehicleSetting();
    void processSetGuideline();
    void processSetTransparentMode();

    //process vehicle data cmd
    void processSetWheelAndGear();
    void processSetSteerWheelAngle();
    void processWheelPulseChooseWheel();
    void processWheelPulseSetPulse(const uint8_t wheelId);
    void processSetGear();

    void getInputKey(int& key);

private:
    bool m_console_status;
    int m_client_fd;
    struct sockaddr_in m_client_addr;
    SvmSignalData m_cmd_data_send;

private:
    const std::string CLIENT_IP = "127.0.0.1";
    const int PORT = 10000;
};
