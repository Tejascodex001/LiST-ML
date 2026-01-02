#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace std;

struct cpusnap {
    string cpu_name;
    uint64_t user_time;
    uint64_t nice_time;
    uint64_t system_kernel_time;
    uint64_t idle_time;
    uint64_t iowait_time;
    uint64_t irq_time;
    uint64_t softirq_time;
};

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

int main (){  
    cpusnap cpuatt, cpuatt1;
    uint64_t total_delta, idle_delta, user_delta, nice_delta, system_delta, iowait_delta, irq_delta, softirq_delta;
    double cpu_usage;

    cpuatt = read_cpu();
    this_thread::sleep_for(1s);
    cpuatt1 = read_cpu();

        cout << "\ndifference" << endl;
        cout << "user_time: " << cpuatt1.user_time - cpuatt.user_time << "\nnice_time:" <<
        cpuatt1.nice_time - cpuatt.nice_time <<  "\nsystem_kern_time:" <<
        cpuatt1.system_kernel_time - cpuatt.system_kernel_time << "\nidle_time: " <<
        cpuatt1.idle_time - cpuatt.idle_time << "\niowait_time: " <<
        cpuatt1.iowait_time - cpuatt.iowait_time << "\nirq_time: " <<
        cpuatt1.irq_time - cpuatt.irq_time << "\nsoftirq_time: " <<
        cpuatt1.softirq_time - cpuatt.softirq_time << endl;

    user_delta = cpuatt1.user_time - cpuatt.user_time;
    nice_delta = cpuatt1.nice_time - cpuatt.nice_time;
    system_delta = cpuatt1.system_kernel_time - cpuatt.system_kernel_time;
    idle_delta = cpuatt1.idle_time - cpuatt.idle_time;
    iowait_delta = cpuatt1.iowait_time - cpuatt.iowait_time;
    irq_delta = cpuatt1.irq_time - cpuatt.irq_time;
    softirq_delta = cpuatt1.softirq_time - cpuatt.softirq_time;

    total_delta = user_delta + nice_delta + system_delta + idle_delta + iowait_delta + irq_delta + softirq_delta;
    cpu_usage = double(total_delta - idle_delta)/double(total_delta);
    cout << "\n" << cpu_usage*100 << "%" << endl;
        
}