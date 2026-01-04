#include <iostream>

#include "proc_reader.hpp"
#include "sampler.hpp"

using namespace std;

int main (){  
    while (true){
    double cpu_usage = sample_cpu_usage(); 
    cout << "cpu usage: " << cpu_usage*100 << "%" << endl;
    }  
}