#include "writer.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>

using namespace std;

Writer::Writer(const string &Filename):header_written_(false){
    file_.open(Filename, ios::app);
    if (!file_.is_open()){
        cerr << "cannot open file" << endl;
        exit(1);
    }
    file_.seekp(0, ios::end);
    if (file_.tellp() == 0){
        file_ << "timestamp,cpu_usage\n";
        header_written_ = true;
    }
}

void Writer::write_sample(const sample &usage){
    file_ << usage.timeStamp << "," << usage.cpu_usage << "\n";
}

void Writer::close(){
    if (file_.is_open()) file_.close();
}