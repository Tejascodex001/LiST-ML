#ifndef WRITER_HPP
#define WRITER_HPP

#include <fstream>
#include <string>

#include "sampler.hpp"

class Writer{
    private:
    std::ofstream file_;
    bool header_written_;

    public:
    Writer(const std::string &Filename);
    void write_sample(const sample &usage);
    void close();
    void flush();
};

#endif