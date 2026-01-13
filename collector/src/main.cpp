#include "proc_reader.hpp"
#include "sampler.hpp"
#include "writer.hpp"
#include "queue.hpp"
#include <cmath>
#include <functional>
#include <thread>
#include <csignal>
#include <atomic>

using namespace std;

std::atomic<bool> stop_request(false);

void handle_sigint(int){
    stop_request.store(true);
}

void writer_thread(Samplequeue &q){
    Writer writer("sample.csv");
    while(true){
        if(stop_request and q.empty()) break;
        sample data = q.pop(stop_request);
        if(stop_request && q.empty()) continue;
        writer.write_sample(data);
    }
    writer.flush();
    writer.close();
}
/*  Sigint handled completely ( I didnt learn much as this concept is difficult and i am burned)
    --Date: 10 Jan 2026.
*/
int main (){  
    vector <sample> window;
    int max_size = 30;
    double cpu_mean = 0, mem_mean = 0, cpu_sum = 0, mem_sum = 0, cpu_var_sum = 0, mem_var_sum = 0;
    double cpu_std = 0, mem_std = 0;

    std::signal(SIGINT, handle_sigint);
    Samplequeue samplequeue;
    thread t(writer_thread, ref(samplequeue));

    //unoptimized, needs optimization -  optimized can be O(n).
    
    while(!stop_request){
        sample data = sample_usage();
        window.push_back(data);
        if (window.size() > max_size){
            window.erase(window.begin());
        }
        if (window.size() < max_size) continue;
        else{
            cpu_sum = 0; mem_sum = 0;
            for (const sample &s: window){
                mem_sum += s.mem_usage;
                cpu_sum += s.cpu_usage;
            }
            mem_mean = mem_sum/window.size();
            cpu_mean = cpu_sum/window.size();
            data.cpu_mean = cpu_mean;
            data.mem_mean = mem_mean;

            cpu_var_sum = 0; mem_var_sum = 0;
            for (const sample &s: window){
                double diffcpu = s.cpu_usage - cpu_mean;
                double diffmem = s.mem_usage - mem_mean;
                cpu_var_sum += diffcpu*diffcpu;
                mem_var_sum += diffmem*diffmem;
            }
            cpu_std = sqrt(cpu_var_sum/window.size());
            mem_std = sqrt(mem_var_sum/window.size());
            data.cpu_std = cpu_std;
            data.mem_std = mem_std;
            samplequeue.push(data);
        }
        
    }
    samplequeue.notifyall();
    t.join();
}