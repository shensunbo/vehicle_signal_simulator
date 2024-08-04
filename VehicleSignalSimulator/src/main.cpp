#include "MessagelProccessor.h"
#include "DemoInterfaceAdapter.h"
#include <iostream>
#include <memory>

int main(int argc, char** argv) {
    SvmSignalProcessor svmSignalProcessor("0.0.0.0", 10000, std::make_shared<DemoInterfaceAdapter>());

    svmSignalProcessor.clientRun();
    svmSignalProcessor.serverRun();

    return 0;
}