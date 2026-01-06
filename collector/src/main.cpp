#include "sampler.hpp"
#include "writer.hpp"

using namespace std;

int main (){  
    Writer writer("sample.csv");

    while (true){
        sample usage = sample_usage(); 
        writer.write_sample(usage);
        writer.flush();
    }  
    writer.close();
}