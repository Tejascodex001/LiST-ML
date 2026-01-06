#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <chrono>

#include "proc_reader.hpp"

using namespace std::chrono_literals;
using namespace std;

cpusnap read_cpu(){
    cpusnap cpuatt;
    string folderpath = "/proc/stat";
    ifstream inputFile(folderpath);
    if(!inputFile.is_open()) { cout << "error not open" << endl; exit(0);}
    else {
        string line;
        getline(inputFile,line);

        istringstream(line) >> cpuatt.cpu_name >> cpuatt.user_time >> 
        cpuatt.nice_time >> cpuatt.system_kernel_time >> cpuatt.idle_time >> cpuatt.iowait_time >> 
        cpuatt.irq_time >> cpuatt.softirq_time;

        inputFile.close();
    }
    return cpuatt;
}