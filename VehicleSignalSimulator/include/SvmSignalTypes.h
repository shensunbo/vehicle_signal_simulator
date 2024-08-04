#pragma once

#include <cstdint>

typedef struct SvmSignalData_t {
    uint8_t fid;
    uint8_t cmd;
    uint8_t value;
    uint8_t data[4];
    uint8_t reserve[9]; //total 16
}SvmSignalData;

//HMI_>SVM
#define SVM_HMI_REQUEST_ACTIVE_GUIDELINE                                          0x00  /* HMI requests to show/hide guidelines in different views                                        */
#define SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL                              0x01  /* HMI requests to enable/disable 4D transparent car model view                                   */

//BCM->TDA4
#define STEERING_WHEEL_ANGLE                                                      0x02  /*Used to update guideline                      */
#define EACH_WHEEL_PULSE_SIGNAL                                                   0x03  /*Used by transparement glass bottom algorithm  */
#define GEAR                                                                      0x04  /*Used to auto change SVM views                 */

#define CREATE_HMI_CLIENT                                                         0xFE  /*call AcfInterfaceAdapter::hmiClientInit*/
#define SIMULATOR_EXIT                                                            0xFF  /*use to end the simulator                      */

enum class SVM_HMI_Request_Active_Guideline_Value{
    SHOW_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW = 0,
    SHOW_GUIDELINE_ONLY_IN_BIRD_EYE_VIEW,
    SHOW_GUIDELINE_ONLY_IN_SINGLE_VIEW,
    HIDE_GUIDELINE_BOTH_IN_BIRD_EYE_VIEW_AND_SINGLE_VIEW,
};


enum class SVM_HMI_Request_Enable_Transparent_Car_Model_Value{
    NO_COMMNADS = 0,
    ENABLE_TRANSPARENT_CAR_MODEL,
    DISABLE_TRANSPARENT_CAR_MODEL,
};

enum class Gear_value{
    P = 0,
    R,
    N,
    D,
}; 