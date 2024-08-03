#ifndef __SVM_SIGNAL_TYPES__
#define __SVM_SIGNAL_TYPES__

#include <cstdint>

typedef struct SvmSignalData_t {
    uint8_t fid;
    uint8_t cmd;
    uint8_t value;
    uint8_t data[4];
    uint8_t reserve[9]; //total 16
}SvmSignalData;

//HMI_>SVM
#define SVM_HMI_REQUEST_DEBUG_CMD                                                 0x00  /* HMI requests to debugging cmds                                                                 */
#define SVM_HMI_REQUEST_ACTIVE_SVM                                                0x01  /* HMI requests to active/deactive SVM service                                                    */
#define SVM_HMI_REQUEST_TOUCH_ACTION                                              0x02  /* HMI sends touch event to SVM service                                                           */
#define SVM_HMI_REQUEST_SWITCH_VIEW_MODE                                          0x03  /* HMI requests to switch Single/3D/Wheel/Wide /Floating view mode(type)                          */
#define SVM_HMI_REQUEST_ACTIVE_GUIDELINE                                          0x04  /* HMI requests to show/hide guidelines in different views                                        */
#define SVM_HMI_REQUEST_ACTIVE_RADAR_ICON                                         0x05  /* HMI requests to show/hide radar icon/wave/wall in different views                              */
#define SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL                              0x06  /* HMI requests to enable/disable 4D transparent car model view                                   */
#define SVM_HMI_REQUEST_CHANGE_CAR_MODEL                                          0x07  /* HMI requests to change 3D car model                                                            */
#define SVM_HMI_REQUEST_ENABLE_AUTO_SWITCH_VIEW_BY_OBSTACLE                       0x08  /* HMI enables/disables auto switch view by obstacle                                              */
#define SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_OBSTACLE                        0x09  /* HMI enables/disables auto active view by obstacle                                              */
#define SVM_HMI_REQUEST_ENABLE_AUTO_ACTIVE_SVM_BY_TURN_ACTION                     0x0A  /* HMI enables/disables auto active view by vehicle turn action                                   */
#define SVM_HMI_REQUEST_START_EOL_CALIBRATION                                     0x0B  /* HMI request to start to enter EOL calibration                                                  */
#define SVM_HMI_REQUEST_START_MANUAL_CALIBRATION                                  0x0C  /* HMI request to start enter manual calibration                                                  */
#define SVM_HMI_REQUEST_ON_VIDEO_LOST                                             0x0D  /* HMI detects SVM output video signal lost/normal and feedback to SVM service                    */
#define SVM_HMI_SELECT_SVM_VIEW                                                   0x0E  /* HMI (or other outside controller) selects the SVM view mode to be activated.                   */

//SVM_>HMI
#define SVM_SERVICE_REPORT_WORKING_STATE                                          0x81  /* SVM service reports current working state                                                      */
#define SVM_SERVICE_REPORT_SHOWN_VIEWS                                            0x82  /* SVM service reports current shown views                                                        */
#define SVM_SERVICE_REPORT_VIEW_MODE                                              0x83  /* SVM service reports current view mode(type)                                                    */
#define SVM_SERVICE_REPORT_GUIDELINE_STATE                                        0x84  /* SVM service reports current guideline on/off setting                                           */
#define SVM_SERVICE_REPORT_RADAR_ICON_STATE                                       0x85  /* SVM service reports current radar icon on/off setting                                          */
#define SVM_SERVICE_REPORT_TRANSPARENT_CAR_MODEL_STATE                            0x86  /* SVM service reports current transparent car mode enabled or not                                */
#define SVM_SERVICE_REPORT_CURRENT_CAR_MODEL                                      0x87  /* SVM service reports current car model                                                          */
#define SVM_SERVICE_REPORT_ENABLED_STATE_FOR_AUTO_SWITCH_VIEW_BY_OBSTACLE         0x88  /* SVM service reports current enables/disables state for auto switching view by obstacle         */
#define SVM_SERVICE_REPORT_ENABLED_STATE_FOR_AUTO_ACTIVE_SVM_BY_OBSTACLE          0x89  /* SVM service reports current enables/disables state for auto activing SVM by obstacle           */
#define SVM_SERVICE_REPORT_ENABLED_STATE_FOR_AUTO_ACTIVE_SVM_BY_TURN_ACTION       0x8A  /* SVM service reports current enables/disables state for auto activing SVM by vehicle turn action*/
#define SVM_SERVICE_REPORT_FRONT_CAMERA_FAULT                                     0x8B  /* SVM service reports current front camera fault                                                 */
#define SVM_SERVICE_REPORT_REAR_CAMERA_FAULT                                      0x8C  /* SVM service reports current rear camera fault                                                  */
#define SVM_SERVICE_REPORT_LEFT_CAMERA_FAULT                                      0x8D  /* SVM service reports current left camera fault                                                  */
#define SVM_SERVICE_REPORT_RIGHT_CAMERA_FAULT                                     0x8E  /* SVM service reports current right camera fault                                                 */
#define SVM_SERVICE_REPORT_GUIDELINE_FAULT                                        0x8F  /* SVM service reports current guideline fault                                                    */
#define SVM_SERVICE_REPORT_SVM_ENABLED_STATE                                      0x90  /* SVM service reports current SVM enabled or disabled                                            */
#define SVM_SERVICE_REPORT_CALIBRATION_DATA_STATE                                 0x91  /* SVM service reports current calibration data available state                                   */
#define SVM_SERVICE_REPORT_EOL_CALIBRATION_PROGRESS                               0x92  /* SVM service reports current EOL calibration progress                                           */
#define SVM_SERVICE_REPORT_MANUAL_CALIBRATION_PROGRESS                            0x93  /* SVM service reports current Manual calibration progress                                        */
#define SVM_SERVICE_REPORT_CALIBRATION_FAULT                                      0x94  /* SVM service reports current calibration fault                                                  */
#define SVM_SERVICE_REQUEST_HMI_VISIBILITY                                        0x95  /* SVM service send requests to show HMI or hide HMI actively in some automatic activing cases.   */

//BCM->TDA4
#define STEERING_WHEEL_ANGLE                                                      0x41  /*Used to update guideline                      */
#define EACH_WHEEL_PULSE_SIGNAL                                                   0x42  /*Used by transparement glass bottom algorithm  */
#define GEAR                                                                      0x43  /*Used to auto change SVM views                 */
#define VEHICLE_SPEED                                                             0x44  /*Used to stop SVM or 3D car model animation    */
#define VEHICLE_BODAY_HEIGHT                                                      0x45  /*Used to do calibration                        */
#define STEERING_LEVER_SIGNAL                                                     0x46  /*Used to auto active SVM                       */
#define VEHICLE_TYPE                                                              0x47  /*Used to config car                            */
#define VEHICLE_VARINT                                                            0x48  /*Used to config car                            */
#define VEHICLE_VIN                                                               0x49  /*Reserved                                      */
#define VEHICLE_LANGUAGE                                                          0x4A  /*Reserved                                      */
#define LEFT_FRONT_WINDOW_OPEN_STATE                                              0x4B  /*Used for 3D car model animation               */
#define RIGHT_FRONT_WINDOW_OPEN_STATE                                             0x4C  /*Used for 3D car model animation               */
#define LEFT_REAR_WINDOW_OPEN_STATE                                               0x4D  /*Used for 3D car model animation               */
#define RIGHT_REAR_WINDOW_OPEN_STATE                                              0x4E  /*Used for 3D car model animation               */
#define RUNROOF_WINDOW_OPEN_STATE                                                 0x4F  /*Used for 3D car model animation               */
#define LEFT_FRONT_DOOR_OPEN_STATE                                                0x50  /*Used for 3D car model animation               */
#define RIGHT_FRONT_DOOR_OPEN_STATE                                               0x51  /*Used for 3D car model animation               */
#define LEFT_REAR_DOOR_OPEN_STATE                                                 0x52  /*Used for 3D car model animation               */
#define RIGHT_REAR_DOOR_OPEN_STATE                                                0x53  /*Used for 3D car model animation               */
#define TRUNK_OPEN_STATE                                                          0x54  /*Used for 3D car model animation               */
#define HOOD_OPEN_STATE                                                           0x55  /*Used for 3D car model animation               */
#define HIGH_BEAM_LIGHT_STATE                                                     0x56  /*Used for 3D car model animation               */
#define LOW_BEAM_HEADLIGHT_STATE                                                  0x57  /*Used for 3D car model animation               */
#define FRONT_WHEEL_PROFILE_LIGHT_STATE                                           0x58  /*Used for 3D car model animation               */
#define REAR_WHEEL_PROFILE_LIGHT_STATE                                            0x59  /*Used for 3D car model animation               */
#define REAR_FOG_LIGHT_STATE                                                      0x5A  /*Used for 3D car model animation               */
#define REVERSE_LIGHT_STATE                                                       0x5B  /*Used for 3D car model animation               */
#define BRAKE_LIGHT_STATE                                                         0x5C  /*Used for 3D car model animation               */
#define LEFT_TURN_SIGNAL                                                          0x5D  /*Used for 3D car model animation               */
#define RIGHT_TURN_SIGNAL                                                         0x5E  /*Used for 3D car model animation               */
#define DAYTIME_RUNNING_LIGHTS                                                    0x5F  /*Used for 3D car model animation               */
#define OBSTACLE_MINIMUM_DISTANCE_OF_EACH_RADAR                                   0x60  /*Used for auto activing SVM by obstacle        */
#define WORK_FAULT_STATE_OF_EACH_RADAR                                            0x61  /*Used for radar wall/icon/wave rendering       */
#define WARNING_STATE_OF_EACH_RADAR                                               0x62  /*Used for radar wall/icon/wave rendering       */
#define WARNING_LEVEL_OF_EACH_RADAR                                               0x63  /*Used for radar wall/icon/wave rendering       */
#define APA_WORKING_STATE                                                         0x64  /*Reserved                                      */
#define PEB_WARNING_STATE                                                         0x65  /*Reserved                                      */
#define RCTA_WARNING                                                              0x66  /*Reserved                                      */

#define CREATE_HMI_CLIENT                                                         0xFE  /*call AcfInterfaceAdapter::hmiClientInit*/
#define SIMULATOR_EXIT                                                            0xFF  /*use to end the simulator                      */

//TDA4->VIP
#define REQUEST_TO_GET_ALL_CACHED_SIGNALS                                         0xF0  /* Get missed signal after SVM service startup */

enum class SVM_HMI_Request_Debug_Cmd_Value{
    DUMP_FRAMES = 0,
    START_VIDEO_RECORDING,
    STOP_VIDEO_RECORDING,
    CAPTURE_SCREEN,
    USE_VIDEO_SIMULATION_CAPTURE,
    USE_PICTURE_SET_SIMULATION_CAPTURE,
    DUMP_MESSAGE,
    COPY_ALL_DEBUG_DATA_TO_USB,
    EXECUTE_REMOTE_COMMAND_STRING,
};

enum class SVM_HMI_Request_Active_SVM_Value{
    NO_COMMANDS = 0,
    ACTIVE,
    DEACTIVE,
};

enum class SVM_HMI_Request_Touch_Action_Value{
    RELEASE = 0,
    PRESSED,
    HOLD,
    RESERVED,
};

enum class SVM_HMI_Request_Switch_View_Mode_Value{
    NO_REQUEST = 0,
    BEV_SINGLE_MODE,
    BEV_3D_MODE,
    BEV_WHEEL_VIEW_MODE,
    WIDE_VIEW_MODE,
    FLOATING_VIEW_MODE,
};

enum class SVM_HMI_Request_Active_Guideline_Value{
    SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW = 0,
    SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW,
    SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW,
    HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
};

enum class SVM_HMI_Request_Active_Radar_Icon_Value{
    SHOW_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW = 0,
    SHOW_RADAR_ICON_ONLY_IN_BIRD_EYE_VIEW,
    SHOW_RADAR_ICON_ONLY_IN_SINGLE_VIEW,
    HIDE_RADAR_ICON_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
};

enum class SVM_HMI_Request_Enable_Transparent_Car_Model_Value{
    NO_COMMNADS = 0,
    ENABLE_TRANSPARENT_CAR_MODEL,
    DISABLE_TRANSPARENT_CAR_MODEL,
};

enum class SVM_HMI_Request_Change_Car_Model_Value{
    CAR_MODEL_1 = 0,
    CAR_MODEL_2,
    CAR_MODEL_3,
};

enum class SVM_HMI_Request_Enable_Auto_Switch_View_By_Obstacle_Value{
    DISABLED = 0,
    ENABLED,
};

enum class SVM_HMI_Request_Enable_Auto_Active_SVM_By_Obstacle_Value{
    DISABLED = 0,
    ENABLED,
};

enum class SVM_HMI_Request_Enable_Auto_Active_SVM_By_Turn_Action_Value{
    DISABLED = 0,
    ENABLED,
};

enum class SVM_HMI_Request_Start_EOL_Calibration_Value{
    NO_REQUEST = 0,
    START_REQUEST,
    STOP_REQUEST,
};

enum class SVM_HMI_Request_Manual_Calibration_Value{
    NO_REQUEST = 0,
    START_REQUEST,
    STOP_REQUEST,
};

enum class SVM_HMI_Request_On_Video_Lost_Value{
    NOMAL = 0,
    VIDEO_LOST,
};

enum class SVM_HMI_Select_SVM_View_Value{
    NO_COMMANDS = 0x00,
    SELECT_3D_FRONT_LOOKING_VIEW_UNDER_3D_VIEW_MODE       = 0x01,
    SELECT_3D_REAR_LOOKING_VIEW_UNDER_3D_VIEW_MODE        = 0x02,
    SELECT_3D_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE        = 0x03,
    SELECT_3D_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE       = 0x04,
    SELECT_3D_FRONT_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE  = 0x05,
    SELECT_3D_FRONT_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE = 0x06,
    SELECT_3D_REAR_LEFT_LOOKING_VIEW_UNDER_3D_VIEW_MODE   = 0x07,
    SELECT_3D_REAR_RIGHT_LOOKING_VIEW_UNDER_3D_VIEW_MODE  = 0x08,
    SELECT_3D_TOP_LOOKING_VIEW_UNDER_3D_VIEW_MODE         = 0x09,

    SELECT_FRONT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE       = 0x10,
    SELECT_REAR_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE        = 0x11,
    SELECT_LEFT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE        = 0x12,
    SELECT_RIGHT_SINGLE_VIEW_UNDER_SINGLE_VIEW_MODE       = 0x13,
    SELECT_4_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE            = 0x14,
    SELECT_FRONT_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE      = 0x15,
    SELECT_REAR_2_WHEELS_VIEW_UNDER_WHEEL_VIEW_MODE       = 0x16,
    SELECT_LEFT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE    = 0x17,
    SELECT_RIGHT_FRONT_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE   = 0x18,
    SELECT_LEFT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE     = 0x19,
    
    SELECT_RIGHT_REAR_WHEEL_VIEW_UNDER_WHEEL_VIEW_MODE    = 0x20,
    SELECT_FRONT_WIDE_VIEW_UNDER_WIDE_VIEW_MODE           = 0x21,
    SELECT_REAR_WIDE_VIEW_UNDER_WIDE_VIEW_MODE            = 0x22,
};


enum class Gear_value{
    P = 0,
    R,
    N,
    D,
}; 

enum class Door_State_value{
    CLOSE = 0,
    OPEN,
};

enum class Light_State_value{
    CLOSE = 0,
    OPEN,
};

enum class Radar_Region_Value {
   /* front radar: front left corner,front left middle,front right middle, front right corner */
   SVM_RADAR_REGION_FRONT_LEFT_CORNER_REGION1 = 0,
   SVM_RADAR_REGION_FRONT_LEFT_CORNER_REGION2 = 1,
   SVM_RADAR_REGION_FRONT_LEFT_MIDDLE_REGION1 = 2,
   SVM_RADAR_REGION_FRONT_LEFT_MIDDLE_REGION2 = 3,
   SVM_RADAR_REGION_FRONT_RIGHT_MIDDLE_REGION1 = 4,
   SVM_RADAR_REGION_FRONT_RIGHT_MIDDLE_REGION2 = 5,
   SVM_RADAR_REGION_FRONT_RIGHT_CORNER_REGION1 = 6,
   SVM_RADAR_REGION_FRONT_RIGHT_CORNER_REGION2 = 7,
   /* side radar : left side =  right side */
   SVM_RADAR_REGION_SIDE_LEFT_REGION1 = 8,
   SVM_RADAR_REGION_SIDE_LEFT_REGION2 = 9,
   SVM_RADAR_REGION_SIDE_LEFT_REGION3 = 10,
   SVM_RADAR_REGION_SIDE_LEFT_REGION4 = 11,
   SVM_RADAR_REGION_SIDE_LEFT_REGION5 = 12,
   SVM_RADAR_REGION_SIDE_RIGHT_REGION1 = 13,
   SVM_RADAR_REGION_SIDE_RIGHT_REGION2 = 14,
   SVM_RADAR_REGION_SIDE_RIGHT_REGION3 = 15,
   SVM_RADAR_REGION_SIDE_RIGHT_REGION4 = 16,
   SVM_RADAR_REGION_SIDE_RIGHT_REGION5 = 17,
   /* rear radar : rear left corner,rear left middle,rear right middle, rear right corner */
   SVM_RADAR_REGION_REAR_LEFT_CORNER_REGION1 = 18,
   SVM_RADAR_REGION_REAR_LEFT_CORNER_REGION2 = 19,
   SVM_RADAR_REGION_REAR_LEFT_MIDDLE_REGION1 = 20,
   SVM_RADAR_REGION_REAR_LEFT_MIDDLE_REGION2 = 21,
   SVM_RADAR_REGION_REAR_RIGHT_MIDDLE_REGION1 = 22,
   SVM_RADAR_REGION_REAR_RIGHT_MIDDLE_REGION2 = 23,
   SVM_RADAR_REGION_REAR_RIGHT_CORNER_REGION1 = 24,
   SVM_RADAR_REGION_REAR_RIGHT_CORNER_REGION2 = 25,
   SVM_RADAR_REGION_MAX_NUM = 26
};


#endif // __SVM_SIGNAL_TYPES__