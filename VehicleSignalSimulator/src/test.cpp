#include "MessagelProccessor.h"
#include <iostream>

int main(int argc, char** argv) {
    SvmSignalProcessor svmSignalProcessor("0.0.0.0", 10000);

    svmSignalProcessor.run();

    return 0;
}