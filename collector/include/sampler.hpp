#ifndef SAMPLER_HPP
#define SAMPLER_HPP

#include <cstdint>
#include <chrono>

struct sample{
    std::time_t timeStamp;
    double cpu_usage;
};

sample sample_usage();



#endif