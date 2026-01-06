#ifndef PROC_READER_HPP
#define PROC_READER_HPP

#include <cstdint>
#include <string>

struct cpusnap {
    std::string cpu_name;
    std::uint64_t user_time;
    std::uint64_t nice_time;
    std::uint64_t system_kernel_time;
    std::uint64_t idle_time;
    std::uint64_t iowait_time;
    std::uint64_t irq_time;
    std::uint64_t softirq_time;
};

cpusnap read_cpu();

#endif