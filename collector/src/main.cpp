#include "sampler.hpp"
#include "writer.hpp"
#include "queue.hpp"
#include <functional>
#include <thread>

using namespace std;

void write(Samplequeue &q){
    Writer writer("sample.csv");
    while(true){
        sample data = q.pop();
        writer.write_sample(data);
        writer.flush();
    }
}
/*  so writer.flush() was called as the csv file was not updating, now i need to implement 
safe shutdown method, which I'll do tomorrow, I'm tired as i learnt many things
    --Date: 7 Jan 2026.
*/
int main (){  
    Samplequeue samplequeue;
    thread t(write, ref(samplequeue));

    while(true){
        sample data = sample_usage();
        samplequeue.push(data);
    }
}