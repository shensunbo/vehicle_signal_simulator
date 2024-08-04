#include "MessagelProccessor.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 

#include "mylog.h"

SvmSignalProcessor::SvmSignalProcessor(std::string ip, int port, std::shared_ptr<InterfaceAdapter> _interfaceAdapter):
    m_ip(ip),
    m_port(port),
    m_socket(-1),
    m_process_done(false),
    m_cmd_line_input_thread(nullptr),
    m_interface_adapter(std::move(_interfaceAdapter))
{
}

SvmSignalProcessor::~SvmSignalProcessor() {
    mylog(LogLevel::D, "~SvmSignalProcessor() start");
    close(m_socket);

    if (m_cmd_line_input_thread != nullptr && m_cmd_line_input_thread->joinable()) {
        m_cmd_line_input_thread->join();
        delete m_cmd_line_input_thread;
        m_cmd_line_input_thread = nullptr;
    }

    mylog(LogLevel::D, "~SvmSignalProcessor() end");
}

void SvmSignalProcessor::clientRun(){
    if(consoleCamImpl.init()){
        mylog(LogLevel::E, "consoleCamImpl init failed");
        MY_ASSERT(0, "consoleCamImpl init failed");
        return;
    }

    //create cmd line input processor thread
    auto cmdLineProcessor = [this]() { return this->consoleCamImpl.execute(); };
    m_cmd_line_input_thread =  new std::thread(cmdLineProcessor);
}
void SvmSignalProcessor::serverRun(){
    if(udpServerInit()){
        mylog(LogLevel::E, "udpServerInit failed");
        MY_ASSERT(0, "udpServerInit failed");
        return;
    }

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
        case SVM_HMI_REQUEST_ACTIVE_GUIDELINE:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ACTIVE_GUIDELINE");
            SVM_HMI_Request_Active_Guideline_Process();
            break;
        case SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL:
            mylog(LogLevel::D, "SVM_HMI_REQUEST_ENABLE_TRANSPARENT_CAR_MODEL");
            SVM_HMI_Request_Enable_Transparent_Car_Model_Process();
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

void SvmSignalProcessor::SVM_HMI_Request_Active_Guideline_Process() const {
    m_interface_adapter->RequestActiveGuideline(m_svm_signal_data_recv.value);
}

void SvmSignalProcessor::SVM_HMI_Request_Enable_Transparent_Car_Model_Process() const {
    m_interface_adapter->RequestEnableTransparentCarModel(m_svm_signal_data_recv.value);
}

void SvmSignalProcessor::SteeringWheelAngleProcess() const{
    int8_t radians = 0;
    int32_t degrees = 0;
    memcpy(&radians, &m_svm_signal_data_recv.value, sizeof(radians));
    mylog(LogLevel::D, "SetSteeringWheelAngle %d", radians);
    degrees = radians * 57; //Radians to degrees
    m_interface_adapter->SetSteeringWheelAngle(degrees);
}

void SvmSignalProcessor::EachWheelPulseSignalProcess() const{
    mylog(LogLevel::D, "SetEachWheelPulseSignal");
    m_interface_adapter->SetEachWheelPulseSignal(m_svm_signal_data_recv.value, m_svm_signal_data_recv.data[0]);
}
void SvmSignalProcessor::GearProcess() const{
    m_interface_adapter->SetGear(m_svm_signal_data_recv.value);
}