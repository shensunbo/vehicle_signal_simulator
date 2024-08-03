#ifndef CONSOLE_CAM_IMPL_H_
#define CONSOLE_CAM_IMPL_H_

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
    void printDebugCmdHelp();
    void printTouchEventHelp();
    void printHmiSettingHelp();
    void printVehicleSettingHelp();
    void printSetViewHelp();
    void printSetViewModeHelp();
    void printSetViewDirectionHelp();
    void printSetGuidelineHelp();
    void printSetRadarModeHelp();
    void printSetTransparentModeHelp();
    void printChangeCarModelHelp();
    void printSetAutoActiveModeHelp();
    void printSetWheelAndGearHelp();
    void printSetSteerWheelAngleHelp();
    void printWheelPulseChooseWheelHelp();
    void printWheelPulseSetPulseHelp();
    void printSetGearHelp();
    void printSetVehicleSpeedHelp();
    void printSetVehicleConfigHelp();
    void printSetCarWindowAndDoorStateHelp();
    void printSetCarLightStateHelp();
    void printSetRadarStateHelp();
    void printChooseRadarHelp();
    void printSetRadarDistanceHelp();
    void printSetParkingSystemHelp();

    //process HMI cmd
    void processKey(int key);
    void processConsoleCamera();
    void processDebugCmd();
    void processTouchEvent();
    void processHmiSetting();
    void processVehicleSetting();
    void processSetView();
    void processSetViewMode();
    void processSetViewDirection();
    void processSetGuideline();
    void processSetRadarMode();
    void processSetTransparentMode();
    void processChangeCarModel();
    void processSetAutoActiveMode();

    //process vehicle data cmd
    void processSetWheelAndGear();
    void processSetSteerWheelAngle();
    void processWheelPulseChooseWheel();
    void processWheelPulseSetPulse(const uint8_t wheelId);
    void processSetGear();
    void processSetVehicleSpeed();
    void processSetVehicleConfig();
    void processSetCarWindowAndDoorState();
    void processSetCarLightState();
    void processSetRadarState();
    void processChooseRadar();
    void processSetRadarDistance(const uint8_t radar_id);
    void processSetParkingSystem();

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


#endif